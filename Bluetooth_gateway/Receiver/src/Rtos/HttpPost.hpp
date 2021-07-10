#ifndef _HTTP_POST_HPP
#define _HTTP_POST_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/NetworkCredentials.hpp"
#include "../Network/JsonBuilder.hpp"
#include "../Network/JsonParser.hpp"
#include "../Utils/TimerMilis.hpp"
#include "../Bluetooth/DeviceList.hpp"
#include "Task.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"
#include "Tasks.hpp"

#include <vector>
#include <memory>
#include <BLEDevice.h>

NAMESPACE_START(Rtos)

class HttpPost : public Task
{
    public:
        HttpPost(std::string serverName, unsigned int networkTimeout = 5000, unsigned int refreshFrequency = 2000);
        void Execute(const int priority = 3, const int stackSize = 5000);
        ~HttpPost() override;
        void InsertData(std::string&& data);
        bool HasReceivedData();
        std::string ReadData();
        
    protected:
        static void Run(void * ownedObject);
        std::string GetData();
        Utils::RadioGuard radioGuard;
        unsigned int networkTimeout = 0;
        bool networkConnected = false;
        QueueHandle_t inQueue;
        std::string serverName;
        Utils::TimerMilis timer;
};

NAMESPACE_END

#endif