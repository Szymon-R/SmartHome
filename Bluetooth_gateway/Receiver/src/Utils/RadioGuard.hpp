#ifndef _RADIO_GUARD_HPP
#define _RADIO_GUARD_HPP

#include "freertos/semphr. h"
#include "Utils.hpp"
#include "TimerMilis.hpp"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "freertos/queue.h"
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
        RadioGuard(const RadioGuard&) = delete;
        RadioGuard(RadioGuard&&) = delete;
        RadioGuard operator= (const RadioGuard&) = delete;

        static RadioGuard& GetInstance();
        bool Acquire(Protocol p, uint32_t timeout = 1000);
        bool Release(Protocol p, uint32_t timeout = 1000);

    private:
        RadioGuard();
        static RadioGuard radio;
        uint32_t bluetoothAgents = 0;
        uint32_t wifiAgents = 0;
        SemaphoreHandle_t mutex = nullptr;
        TimerMilis timer;

};

NAMESPACE_END

#endif