#include "Timer.hpp"

using namespace Drivers;


unsigned long Timer::milis = 0;
unsigned long Timer::seconds = 0;
Timer Timer::timer;

Timer& Timer::GetInstance()
{
    return Timer::timer;
}

unsigned long Timer::GetMilis()
{
    return this->milis;
}

unsigned long Timer::GetSeconds()
{
    return this->seconds;
}

Timer::Timer()
{
    const esp_timer_create_args_t periodic_timer_args = 
    {
        &Timer::TimerCallback,
        nullptr,
        ESP_TIMER_TASK,
        "Timer1"
    };
    esp_timer_create(&periodic_timer_args, &this->timerHandle);

    esp_timer_start_periodic(this->timerHandle, Timer::RESOLUTION);
}

void Timer::TimerCallback(void* arg)
{
    Timer::milis++;
    if(milis % 1000)
    {
        Timer::seconds++;
    }
}