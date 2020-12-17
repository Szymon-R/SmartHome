#include "Tasks.hpp"

using namespace Rtos;

ReadOnce::ReadOnce(Bluetooth::Device& dev) : dev(dev)
{
    this->statusQueue =  xQueueCreate(10, sizeof(Status));
}

void ReadOnce::Execute(const int priority, const int stackSize)
{
    if (xTaskCreate(ReadOnce::Run,
                    "ReadOnce",
                    stackSize,
                    (void*)this,
                    priority,
                    this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task!!");
    }
}

void ReadOnce::Run(void* ownedObject)
{
    ReadOnce* caller = reinterpret_cast<ReadOnce*>(ownedObject);
    xQueueSend(caller->statusQueue, &caller->status.OK, 0);
}

Status ReadOnce::GetLastStatus()
{
    return this->
}

ReadOnce::~ReadOnce()
{
    if (this->taskHandle)
    {
        vTaskDelete(this->taskHandle);
    }
}