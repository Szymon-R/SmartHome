#ifndef _HTTP_HANDLER_HPP
#define _HTTP_HANDLER_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/NetworkCredentials.hpp"
#include "../Utils/TimerMilis.hpp"
#include "Task.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"
#include "Tasks.hpp"

#include <vector>
#include <memory>
#include <BLEDevice.h>

NAMESPACE_START(Rtos)

class HttpHandler : public Task
{
    public:
        HttpHandler(std::string serverName, unsigned int networkTimeout = 5000, unsigned int refreshFrequency = 5000);
        void Execute(const int priority = 3, const int stackSize = 100000);
        std::string GetData();
        void InsertData(const std::string& data);
        ~HttpHandler();
        
    protected:
        static void Run(void * ownedObject);
        unsigned int networkTimeout = 0;
        unsigned int refreshFrequency = 0;
        bool networkConnected = false;
        std::string serverName;
        QueueHandle_t inQueue;
        Utils::TimerMilis timer;
};

NAMESPACE_END

#endif