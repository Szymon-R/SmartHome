#ifndef _TIMER_MILIS_HPP
#define _TIMER_MILIS_HPP

#include "Logger.hpp"
#include "../Drivers/Timer.hpp"

NAMESPACE_START(Utils)

class TimerMilis
{
    public:
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