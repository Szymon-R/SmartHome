#ifndef _HTTP_HANDLER_HPP
#define _HTTP_HANDLER_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/NetworkCredentials.hpp"
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
        HttpHandler();
        void Execute(const int priority = 3, const int stackSize = 100000);
        std::string GetData();
        void InsertData(const std::string& data);
        ~HttpHandler();
        
    protected:
        static void Run(void * ownedObject);
        bool connectionStatus = false;
        QueueHandle_t inQueue;
};

NAMESPACE_END

#endif