#ifndef _TIMER_HPP
#define _TIMER_HPP

#include <FreeRTOS/timers.h>
#include "Logger.hpp"
#include "../Drivers/Timer.hpp"

NAMESPACE_START(Utils)

class TimerMilis
{
    public:
        TimerMilis();
        void Start(unsigned int period);
        void Reset();
        bool IsExpired();
        void Stop();
        unsigned int GetPeriod();
        void SetPeriod(unsigned int period);

    private:
        unsigned int startTime = 0;
        unsigned int period = 0;
        bool isrunning = false;

};

NAMESPACE_END

#endif