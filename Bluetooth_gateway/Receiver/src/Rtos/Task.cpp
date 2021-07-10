#include "Task.hpp"

using namespace Rtos;

Task::Task()
{
    this->statusQueue =  xQueueCreate(Task::QUEUE_SIZE, sizeof(int*));
}

void Task::InsertStatus(Status status)
{
    int* a = new int;
    *a = static_cast<int>(status);
    xQueueSend(this->statusQueue, (void*)&a, 0);
}

Status Task::GetLastStatus()
{
    int* buffer = nullptr;
    if (xQueueReceive(this->statusQueue, &buffer, 0))
    {
        int a = *buffer;
        LOG_LOW("Status received: ", a, "\n\r");
        delete buffer;
        return static_cast<Status>(a);
    }
    return Status::NO_STATUS;
}

Task::~Task()
{
    LOG_LOW("Task destructor called\r\n");
    while (uxQueueSpacesAvailable(this->statusQueue) != Task::QUEUE_SIZE)
    {
        int* buffer = nullptr;
        xQueueReceive(this->statusQueue, &buffer, 0);
        if (!buffer)
        {
            LOG_HIGH("Empty element received from queue\r\n");
        }
        else
        {
            delete buffer;
        }
    }
    if (this->taskHandle != nullptr)
    {
        vTaskDelete(this->taskHandle);
        this->taskHandle = nullptr;
    }
}