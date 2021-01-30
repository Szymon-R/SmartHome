#ifndef _TIMER_HPP
#define _TIMER_HPP

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#include "../Utils/Logger.hpp"

NAMESPACE_START(Drivers)

class Timer
{
    public:
        static Timer& GetInstance();
        unsigned long GetMilis();
        unsigned long GetSeconds();
        Timer(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer operator=(Timer&) = delete;

    private:
        Timer();
        static void TimerCallback(void* arg);
        // Default resolution is 1us, so 1000 means 1ms.
        static constexpr unsigned int RESOLUTION = 1000;
        static Timer timer;
        esp_timer_handle_t timerHandle = nullptr;
        static unsigned long milis;
        static unsigned long seconds;
};

NAMESPACE_END

#endif