#include "RadioGuard.hpp"

using namespace Utils;

RadioGuard RadioGuard::radio;

RadioGuard::RadioGuard()
{
    this->mutex = xSemaphoreCreateMutex();
    if (!this->mutex)
    {
        LOG_HIGH("Couldn't create mutex in RadioGuard class\n\r");
    }
}

RadioGuard& RadioGuard::GetInstance()
{
    return RadioGuard::radio;
}

bool RadioGuard::Acquire(Protocol p, uint32_t timeout = 1000)
{
    if (xSemaphoreTake(this->mutex, timeout))
    {
        timer.Start(timeout);
        if (p = Protocol::BLUETOOTH)
        {
            
        }
    }

    else
    {
        LOG_HIGH("Couldn't acquire mutex in RadioGuard class\n\r");
        return false;
    }
    

}
        bool Release(Protocol p, uint32_t timeout = 1000);

    private:
        RadioGuard() = default;
        static RadioGuard radio;
        uint32_t bluetoothAgents = 0;
