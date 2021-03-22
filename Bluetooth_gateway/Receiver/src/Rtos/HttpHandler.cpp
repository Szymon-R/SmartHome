#include "HttpHandler.hpp"
#include <WiFi.h>
#include <HTTPClient.h>

using namespace Rtos;

HttpHandler::HttpHandler(std::string serverName, unsigned int networkTimeout, unsigned int refreshFrequency)
{
    this->statusQueue =  xQueueCreate(HttpHandler::QUEUE_SIZE, sizeof(int*));
    this->inQueue =  xQueueCreate(HttpHandler::QUEUE_SIZE, sizeof(std::string*));
    this->networkTimeout = networkTimeout;
    this->serverName = serverName;
    this->refreshFrequency = refreshFrequency;
}

void HttpHandler::Execute(const int priority, const int stackSize)
{
    int result = xTaskCreate(HttpHandler::Run,
                "HttpHandler",
                stackSize,
                (void*)this,
                priority,
                this->taskHandle
                );
    if (result != pdPASS)
    {
        LOG_HIGH("Couldn't create task: HttpHandler. Reason: ",result,"\r\n");
    }
    
}

void HttpHandler::InsertData(const std::string& data)
{
    std::string *data2  = new std::string(data);
    xQueueSend(this->inQueue, (void*)&data2, 0); 
}


bool HttpHandler::HasReceivedData()
{
    return uxQueueMessagesWaiting(this->inQueue) != 0;
}

std::string HttpHandler::GetData()
{
    std::string* buffer = nullptr;
    xQueueReceive(this->inQueue, &buffer, 0);
    std::string out = *buffer;
    delete buffer;
    return out;
}

HttpHandler::~HttpHandler()
{
    LOG_LOW("Handle destructor called\r\n");
    while (uxQueueSpacesAvailable(this->inQueue) != HttpHandler::QUEUE_SIZE)
    {
        LOG_LOW("Deleting item\r\n");
        std::string* buffer = nullptr;
        xQueueReceive(this->inQueue, &buffer, 0);
        if (!buffer)
        {
            LOG_HIGH("Empty element received from queue\r\n");
        }
        else
        {
            delete buffer;
            LOG_LOW("Item deleted\r\n");
        }
    }
    if (this->taskHandle)
    {
        vTaskDelete(this->taskHandle);
    }
}

void HttpHandler::Run(void * ownedObject)
{
    static constexpr int IDLE = 0;
    static constexpr int INIT = 1;
    static constexpr int RUN = 2;
    static constexpr int EXIT = 3;

    HttpHandler* caller = reinterpret_cast<HttpHandler*>(ownedObject);
    int state = INIT;
    while (1)
    {
        switch (state)
        {
            case IDLE:
            {
                caller->timer.Start(caller->refreshFrequency);
                if (caller->timer.IsExpired())
                {
                    caller->timer.Stop();
                    if (caller->HasReceivedData())
                    {
                        state = INIT;
                    }
                    else
                    {
                        LOG_MEDIUM("Didn't receive data\n\r");
                    }

                }
                else
                {
                    vTaskDelay(20);
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
                            state = IDLE;
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
                    state = IDLE;
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
                int httpResponseCode = http.POST(caller->GetData().c_str());
                LOG_MEDIUM("HTTP Response code: ", httpResponseCode, "\n\r");
                    
                http.end();
                state = EXIT;
            }
            break;

            case EXIT:
            {
                caller->radioGuard.Release(Utils::Protocol::WIFI, 3000);
                state = IDLE;
            }
            break;

            default:
            break;
        }
    }
}
