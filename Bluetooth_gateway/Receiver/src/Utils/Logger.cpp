#include "Logger.hpp"

using namespace Utils;

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::Uart0::Init()
{
    if(this->initialized == false)
    {
        ESP_ERROR_CHECK(uart_driver_install(this->uartPort, this->BUFFER_SIZE,this->BUFFER_SIZE, this->queueSize, &this->uartQueue, this->interruptFlags));              
        ESP_ERROR_CHECK(uart_param_config(this->uartPort, &this->uartConfig));
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, this->rtsPin, this->ctsPin));
        this->initialized = true;
    }
}

Logger::Logger()
{
    ;
}

Logger::Uart0::Uart0()
{
    this->mutex = xSemaphoreCreateMutex();
}

bool Logger::Transmit()
{
    ;
}

void Logger::Initialize()
{
    this->uart0.Init();
}

int Logger::Uart0::Transmit(const char* src, size_t size)
{
    int var = uart_write_bytes(uart_port_t::UART_NUM_0, src, size);
    return var;
}