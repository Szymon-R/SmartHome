#ifndef _HTTP_POST_HPP
#define _HTTP_POST_HPP

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

class HttpPost : public Task
{
    public:
        HttpPost(std::string serverName, unsigned int networkTimeout = 5000, unsigned int refreshFrequency = 2000);
        void Execute(std::string data, const int priority = 3, const int stackSize = 5000);
        ~HttpPost() override;
        
    protected:
        static void Run(void * ownedObject);
        Utils::RadioGuard radioGuard;
        unsigned int networkTimeout = 0;
        bool networkConnected = false;
        std::string serverName;
        std::string data;
        Utils::TimerMilis timer;
};

NAMESPACE_END

#endif