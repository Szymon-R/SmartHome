#include "HttpHandler.hpp"
#include <WiFi.h>
#include <HTTPClient.h>

using namespace Rtos;

HttpHandler::HttpHandler()
{
    this->statusQueue =  xQueueCreate(HttpHandler::QUEUE_SIZE, sizeof(int*));
    this->inQueue =  xQueueCreate(HttpHandler::QUEUE_SIZE, sizeof(std::string*));
}

void HttpHandler::Execute(const int priority, const int stackSize)
{
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
    LOG_LOW("There are: ", HttpHandler::QUEUE_SIZE - uxQueueSpacesAvailable(this->statusQueue), " elements on queue\r\n");
    while (uxQueueSpacesAvailable(this->statusQueue) != HttpHandler::QUEUE_SIZE)
    {
        LOG_LOW("Deleting item\r\n");
        int* buffer = nullptr;
        xQueueReceive(this->statusQueue, &buffer, 0);
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
    HttpHandler* caller = reinterpret_cast<HttpHandler*>(ownedObject);
    unsigned long lastTime = 0;
    // Timer set to 10 minutes (600000)
    //unsigned long timerDelay = 600000;
    // Set timer to 5 seconds (5000)
    unsigned long timerDelay = 500;


    //Your Domain name with URL path or IP address with path
    const char* serverName = "http://192.168.1.2:1880/update-sensor";

    WiFi.begin(SSID, PASSWORD);
    LOG_MEDIUM("Connecting\n\r");
    while(WiFi.status() != WL_CONNECTED) 
    {
        caller->connectionStatus = true;
        delay(500);
    }
    LOG_MEDIUM("Connected\n\r");

    while(1)
    {
        if ((millis() - lastTime) > timerDelay) 
        {
            LOG_MEDIUM("Trying to connect\n\r");
            //Check WiFi connection status
            if(WiFi.status() == WL_CONNECTED)
            {
                
                LOG_MEDIUM("No longer trying\n\r");
                HTTPClient http;
                caller->InsertStatus(Status::CONNECTED);
                LOG_MEDIUM("Status inserted\n\r");
                // Your Domain name with URL path or IP address with path
                http.begin(serverName);

                // Specify content-type header
                http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                // Data to send with HTTP POST
                //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";           
                // Send HTTP POST request
                //int httpResponseCode = http.POST(httpRequestData);
                
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
                LOG_MEDIUM("WiFi Disconnected\n\r");
            }
        }
        lastTime = millis();
    }
}
