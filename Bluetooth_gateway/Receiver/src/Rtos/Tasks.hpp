#include "../Bluetooth/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include "../Bluetooth/Logger.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"

#include <vector>
#include <memory>

#ifndef _TASKS_HPP
#define _TASKS_HPP

NAMESPACE_START(Rtos)

struct Status 
{
    const int OK = 1;
    const int TIMEOUT = 2;
    const int VALUE_READ = 3;
    const int DEVICE_NOT_FOUND = 4;
    const int SERVICE_NOT_FOUND = 5;
    const int CHARACTERISTIC_NOT_FOUND = 6;
};


class ReadOnce
{
    public:
        ReadOnce(Bluetooth::Device& dev);
        QueueHandle_t statusQueue;
        void Execute(const int priority = 10, const int stackSize = 200);
        Status GetLastStatus();
        ~ReadOnce();

    private:
        Status status;
        static void Run(void * ownedObject);
        Bluetooth::Device& dev;
        TaskHandle_t *taskHandle = nullptr;
};

NAMESPACE_END

# endif