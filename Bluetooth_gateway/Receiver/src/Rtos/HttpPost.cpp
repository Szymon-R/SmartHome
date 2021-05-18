#include "HttpPost.hpp"
#include <WiFi.h>
#include <HTTPClient.h>

using namespace Rtos;

HttpPost::HttpPost(std::string serverName, unsigned int networkTimeout, unsigned int refreshFrequency)
{
    this->inQueue =  xQueueCreate(HttpPost::QUEUE_SIZE, sizeof(std::string*));
    this->networkTimeout = networkTimeout;
    this->serverName = serverName;
}

void HttpPost::Execute(const int priority, const int stackSize)
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
        LOG_HIGH("Couldn't create task: HttpPost. Reason: ", result, "\r\n");
    }
}

HttpPost::~HttpPost()
{
    LOG_LOW("HttpPost destructor called\r\n");
    while (uxQueueSpacesAvailable(this->inQueue) != HttpPost::QUEUE_SIZE)
    {
        std::string* buffer = nullptr;
        xQueueReceive(this->inQueue, &buffer, 0);
        if (!buffer)
        {
            LOG_HIGH("Empty element received from queue\r\n");
        }
        else
        {
            delete buffer;
        }
    }
    if (this->taskHandle)
    {
        vTaskDelete(this->taskHandle);
    }
}

void HttpPost::Run(void * ownedObject)
{
    static constexpr int IDLE = 0;
    static constexpr int INIT = 1;
    static constexpr int RUN = 2;
    static constexpr int EXIT = 3;
    static constexpr int FINALIZE = 4;

    HttpPost* caller = reinterpret_cast<HttpPost*>(ownedObject);
    int state = IDLE;
    while (1)
    {
        switch (state)
        {
            case IDLE:
            {
                if (caller->HasReceivedData())
                {
                    LOG_MEDIUM("Http handler received json data\n\r");
                    state = INIT;
                }
                else
                {
                    //LOG_MEDIUM("Didn't receive data\n\r");
                    vTaskDelay(200);
                }
            }
            break;

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
                std::string data(caller->GetData());
                int httpResponseCode = http.POST(data.c_str());
                LOG_MEDIUM("HTTP Response code: ", httpResponseCode, "\n\r");
                    
                http.end();
                state = FINALIZE;
            }
            break;

            case FINALIZE:
            {
                caller->radioGuard.Release(Utils::Protocol::WIFI, 3000);
                state = IDLE;
            }
            break;

            case EXIT:
            {
                state = IDLE;
                break;
            }
            break;

            default:
            break;
        }
    }
}

void HttpPost::InsertData(std::string&& data)
{
    std::string *data2  = new std::string(std::move(data));
    xQueueSend(this->inQueue, (void*)&data2, 0); 
}


bool HttpPost::HasReceivedData()
{
    return uxQueueMessagesWaiting(this->inQueue) != 0;
}

std::string HttpPost::GetData()
{
    std::string* buffer = nullptr;
    xQueueReceive(this->inQueue, &buffer, 0);
    std::string out = "";
    if (buffer != nullptr)
    {
        out = *buffer;
        delete buffer;
    }
    return out;
}