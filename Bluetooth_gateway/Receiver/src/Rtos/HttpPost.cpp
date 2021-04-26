#include "HttpPost.hpp"
#include <WiFi.h>
#include <HTTPClient.h>

using namespace Rtos;

HttpPost::HttpPost(std::string serverName, unsigned int networkTimeout, unsigned int refreshFrequency)
{
    this->statusQueue =  xQueueCreate(HttpHandler::QUEUE_SIZE, sizeof(int*));
    this->networkTimeout = networkTimeout;
    this->serverName = serverName;
}

void HttpPost::Execute(std::string data, const int priority, const int stackSize) : 
    data(std::move(data))
{
    int result = xTaskCreate(HttpPost::Run,
                "HttpPost",
                stackSize,
                (void*)this,
                priority,
                this->taskHandle
                );
    if (result != pdPASS)
    {
        LOG_HIGH("Couldn't create task: HttpPost. Reason: ",result,"\r\n");
    }
}

HttpPost::~HttpPost()
{
    LOG_LOW("HttpPost destructor called\r\n");
}

void HttpPost::Run(void * ownedObject)
{
    static constexpr int IDLE = 0;
    static constexpr int INIT = 1;
    static constexpr int RUN = 2;
    static constexpr int EXIT = 3;
    static constexpr int FINALIZE = 4;

    HttpPost* caller = reinterpret_cast<HttpPost*>(ownedObject);
    int state = INIT;
    while (1)
    {
        switch (state)
        {
            case INIT:
            {
                if (caller->radioGuard.Acquire(Utils::Protocol::WIFI, 3000))
                {
                    WiFi.begin(SSID, PASSWORD);  
                    caller->timer.Start(caller->networkTimeout);
                    while(WiFi.status() != WL_CONNECTED) 
                    {
                        caller->networkConnected = true;
                        if(caller->timer.IsExpired())
                        {
                            caller->timer.Stop();
                            caller->InsertStatus(Status::TIMEOUT);
                            state = EXIT;
                            break;
                        }
                        vTaskDelay(100);
                    }
                    if (caller->networkConnected)
                    {
                        caller->timer.Stop();
                        caller->InsertStatus(Status::CONNECTED);
                        caller->networkConnected = false;
                        LOG_MEDIUM("Status inserted\n\r");
                        state = RUN;
                    }
                }
                else
                {
                    state = EXIT;
                }
            }
            break;

            case RUN:
            {
                HTTPClient http;
                // Your Domain name with URL path or IP address with path
                http.begin(caller->serverName.c_str());
                
                // If you need an HTTP request with a content type: application/json, use the following:
                http.addHeader("Content-Type", "application/json");
               // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
                int httpResponseCode = http.POST(caller->data.c_str());
                LOG_MEDIUM("HTTP Response code: ", httpResponseCode, "\n\r");
                    
                http.end();
                state = EXIT;
            }
            break;

            case FINISH:
            {
                caller->radioGuard.Release(Utils::Protocol::WIFI, 3000);
                state = IDLE;
            }

            case EXIT:
            {
                caller->radioGuard.Release(Utils::Protocol::WIFI, 3000);
                state = IDLE;
            }
            break;

            case 

            default:
            break;
        }
    }
}
