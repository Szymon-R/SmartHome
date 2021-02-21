#include "RadioGuard.hpp"

using namespace Utils;

RadioGuard::RadioSynchro RadioGuard::radio;

bool RadioGuard::Acquire(Protocol p, uint32_t timeout)
{
    this->timer.Start(timeout);
    bool out = false;
    while (1)
    {
        if (this->radio.Acquire(p, timeout))
        {
            out = true;
            break;
        }
        else if(timer.IsExpired())
        {
            out = false;
            break;
        }
        vTaskDelay(50);
    }
    timer.Stop();
    return out;
}

bool RadioGuard::Release(Protocol p, uint32_t timeout)
{
    timer.Start(timeout);
    bool out = false;
    while (1)
    {
        if (this->radio.Release(p, timeout))
        {
            out = true;
            break;
        }
        else if(timer.IsExpired())
        {
            out = false;
            break;
        }
        vTaskDelay(50);
    }
    timer.Stop();
    return out;
}

RadioGuard::RadioSynchro::RadioSynchro()
{
    mutex = xSemaphoreCreateMutex();
}

bool RadioGuard::RadioSynchro::Acquire(Protocol p, uint32_t timeout)
{
  //  LOG_LOW("For: ", static_cast<int>(p), "\n\r");
    bool out = false;
   /* if (xSemaphoreTake(this->mutex, 100) == pdTRUE)
    {
        LOG_LOW("Mutex taken\n\r");
        if (p == Protocol::BLUETOOTH)
        {
            if (this->wifiAgents == 0)
            {
                if (bluetoothAgents++ == 0)
                {
                    //BLEDevice::init("");
                }
                out = true;
            }
        }
        else if (p == Protocol::WIFI)
        {
            if (this->bluetoothAgents == 0)
            {
                if (wifiAgents++ == 0)
                {
                   // WiFi.enableSTA(true);
                }
                out = true;
            }
        }
        xSemaphoreGive(this->mutex);
    }*/
    LOG_LOW("Result: ", out, "\n\r");
    return out;  
}

bool RadioGuard::RadioSynchro::Release(Protocol p, uint32_t timeout)
{
    LOG_LOW("Release protocol for: ", static_cast<int>(p), "\n\r");
    bool out = false;
    if (xSemaphoreTake(this->mutex, 100) == pdTRUE)
    {
        if (p == Protocol::BLUETOOTH)
        {
            bluetoothAgents--;
            if (bluetoothAgents < 0)
            {
                LOG_HIGH("Bluetooth device released, without taking!\n\r");
            }
            else if (bluetoothAgents == 0)
            {
                //BLEDevice::deinit(false);
            }
            out = true;
        }
        else if (p == Protocol::WIFI)
        {
            wifiAgents--;
            if (wifiAgents < 0)
            {
                LOG_HIGH("Wifi device released, without taking!\n\r");
            }
            else if (wifiAgents == 0)
            {
               // WiFi.enableSTA(false);
            }
            out = true;
        }
        xSemaphoreGive(this->mutex);
    }
    LOG_LOW("Result: ", out, "\n\r");
    return out;  
}

