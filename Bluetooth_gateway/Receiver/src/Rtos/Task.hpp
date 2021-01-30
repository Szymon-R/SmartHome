#ifndef _TASK_HPP
#define _TASK_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"

#include <vector>
#include <memory>

NAMESPACE_START(Rtos)

enum class Status : int
{
    NO_STATUS,
    UPDATE_ERROR,
    OK,
    TIMEOUT,
    VALUE_READ,
    DEVICE_NOT_FOUND,
    SERVICE_NOT_FOUND,
    NOT_CONNECTED,
    CONNECTED,
    CHARACTERISTIC_NOT_FOUND
};

class Task
{
    public:
        virtual void Execute(const int priority = 3, const int stackSize = 100000) = 0;
        Status GetLastStatus();
        QueueHandle_t statusQueue;
        
    protected:
        static constexpr int QUEUE_SIZE = 10;
        Status lastStatus;
        void InsertStatus(Status status);
        TaskHandle_t *taskHandle = nullptr;
};

NAMESPACE_END

#endif