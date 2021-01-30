#include "TimerMilis.hpp"

void TimerMilis::Start(unsigned int period)
{
    if (!this->isrunning)
    {
        this->period = period;
        this->startTime = Drivers::Timer::GetInstance().GetTime();
        this->isrunning = true;
    }
    else
    {
        LOG_MEDIUM("Couldn't start. Timer is already running\n\r");
    }
    

}

void TimerMilis::Reset()
{
    if(this->isrunning)
    {
        this->startTime = Drivers::Timer::GetInstance().GetTime();
    }
    else
    {
        LOG_MEDIUM("Couldn't reset. Timer is not running\n\r");
    }
}

bool TimerMilis::IsExpired()
{
    bool result = false;
    if (!this->isrunning)
    {
        LOG_MEDIUM("Couldn't check if expired. Timer is not running\n\r");
    }
    else
    {
        result = (Drivers::Timer::GetInstance().GetTime() >= (this->startTime + this->period));
    }
    return result;
}

unsigned int TimerMilis::GetPeriod()
{
    return this->period;
}

void TimerMilis::GetPeriod(unsigned int period)
{
    this->period = period;
}

void TimerMilis::Stop()
{
    this->isrunning =  false;
}