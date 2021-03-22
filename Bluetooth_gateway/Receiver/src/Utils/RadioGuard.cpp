#include "RadioGuard.hpp"
#include "esp_wifi.h"

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
    bool out = false;
    if (xSemaphoreTake(this->mutex, 100) == pdTRUE)
    {
        //LOG_HIGH("Mutex taken\n\r");
        if (p == Protocol::BLUETOOTH)
        {
            if (this->wifiAgents == 0)
            {
                if (bluetoothAgents++ == 0)
                {
                    LOG_HIGH("Bluetooth init\n\r");
                    BLEDevice::init("");
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
                    LOG_HIGH("Wifi init\n\r");
                   // wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
                  //  esp_wifi_init(&cfg);
                    WiFi.mode(WIFI_MODE_STA);
                }
                out = true;
            }
        }
        xSemaphoreGive(this->mutex);
        
    }
    return out;  
}

bool RadioGuard::RadioSynchro::Release(Protocol p, uint32_t timeout)
{
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
                LOG_HIGH("Bluetooth deinit\n\r");
                BLEDevice::deinit(false);
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
                LOG_HIGH("Wifi deinit\n\r");
                WiFi.mode(WIFI_OFF);
                esp_wifi_stop();
                //esp_wifi_deinit();
            }
            out = true;
        }
        vTaskDelay(200);
        xSemaphoreGive(this->mutex);
    }
    return out;  
}

