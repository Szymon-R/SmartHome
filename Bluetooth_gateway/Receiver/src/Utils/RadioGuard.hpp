#ifndef _RADIO_GUARD_HPP
#define _RADIO_GUARD_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "Utils.hpp"
#include "TimerMilis.hpp"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "freertos/queue.h"
#include <WiFi.h>
#include <BLEDevice.h>
#include <type_traits>
#include <string>
#include <sstream>

NAMESPACE_START(Utils)

enum class Protocol
{
    BLUETOOTH,
    WIFI
};


class RadioGuard
{
    public:
        RadioGuard() =  default;
        bool Acquire(Protocol p, uint32_t timeout = 5000);
        bool Release(Protocol p, uint32_t timeout = 5000);

    private:
        class RadioSynchro
        {
            public:
                RadioSynchro();
                RadioSynchro(const RadioSynchro&) = delete;
                RadioSynchro(RadioSynchro&&) = delete;
                RadioSynchro operator= (const RadioSynchro&) = delete;

                bool Acquire(Protocol p, uint32_t timeout);
                bool Release(Protocol p, uint32_t timeout);

            private:
                uint32_t bluetoothAgents = 0;
                uint32_t wifiAgents = 0;
                SemaphoreHandle_t mutex = nullptr;
        };
        static RadioSynchro radio;
        TimerMilis timer;
};

NAMESPACE_END

#endif