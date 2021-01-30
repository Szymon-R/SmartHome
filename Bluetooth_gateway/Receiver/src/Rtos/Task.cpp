#include "Task.hpp"

using namespace Rtos;

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