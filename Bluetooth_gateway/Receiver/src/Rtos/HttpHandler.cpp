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
    LOG_HIGH("Start of task Http handler\r\n");
        if (xTaskCreate(HttpHandler::Run,
                    "HttpHandler",
                    stackSize,
                    (void*)this,
                    priority,
                    this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task: HttpHandler\r\n");
    }
    LOG_HIGH("End of task Http handler\r\n");
}

void HttpHandler::InsertData(const std::string& data)
{
    std::string *data2  = new std::string;
    *data2 = data;
    xQueueSend(this->statusQueue, (void*)&data2, 0); 
}

std::string HttpHandler::GetData()
{
    return "";
}

HttpHandler::~HttpHandler()
{
    LOG_LOW("Handle destructor called\r\n");
    while (uxQueueSpacesAvailable(this->inQueue) != HttpHandler::QUEUE_SIZE)
    {
        LOG_LOW("Deleting item\r\n");
        int* buffer = nullptr;
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
    LOG_LOW("Starting task http handler\n\r");
    HttpHandler* caller = reinterpret_cast<HttpHandler*>(ownedObject);
    WiFi.begin(SSID, PASSWORD);

    caller->timer.Start(caller->networkTimeout);

    while(WiFi.status() != WL_CONNECTED) 
    {
        caller->networkConnected = true;
        if(caller->timer.IsExpired())
        {
            caller->InsertStatus(Status::TIMEOUT);
            vTaskSuspend(NULL);
        }
        vTaskDelay(500);
    }

    LOG_MEDIUM("Connected to network\n\r");
    caller->timer.Stop();
    caller->timer.Start(caller->refreshFrequency);
    while(1)
    {
        if (caller->timer.IsExpired()) 
        {
            caller->timer.Reset();
            //Check WiFi connection status
            if(WiFi.status() == WL_CONNECTED)
            {
                
                LOG_MEDIUM("No longer trying\n\r");
                HTTPClient http;
                caller->InsertStatus(Status::CONNECTED);
                LOG_MEDIUM("Status inserted\n\r");
                // Your Domain name with URL path or IP address with path
                http.begin(caller->serverName.c_str());
                
                // If you need an HTTP request with a content type: application/json, use the following:
                http.addHeader("Content-Type", "application/json");
                int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

                // If you need an HTTP request with a content type: text/plain
                //http.addHeader("Content-Type", "text/plain");
                //int httpResponseCode = http.POST("Hello, World!");
                
                LOG_MEDIUM("HTTP Response code: ", httpResponseCode, "\n\r");
                    
                // Free resources
                http.end();
            }
            else 
            {
                caller->InsertStatus(Status::DISCONNECTED);
                caller->timer.Stop();
                LOG_MEDIUM("WiFi Disconnected\n\r");
            }
        }
    }
}
