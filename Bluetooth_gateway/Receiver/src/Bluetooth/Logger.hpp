#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "../../Utils.hpp"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "queue.h"

NAMESPACE_START(Bluetooth)



class Logger
{
    public:
        Logger& GetInstance();
        bool Transmit();

    private:
        class Uart0
        {
            public:
                static constexpr uart_config_t uartConfig = 
                {
                    .baud_rate = 115200,
                    .data_bits = UART_DATA_8_BITS,
                    .parity = UART_PARITY_DISABLE,
                    .stop_bits = UART_STOP_BITS_1,
                    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
                    .rx_flow_ctrl_thresh = 122,
                };
                uart_port_t uartPort = uart_port_t::UART_NUM_0;
                static constexpr int queueSize = 10;
                static constexpr int interruptFlags = 0;
                static constexpr int rtsPin = 18;
                static constexpr int ctsPin = 19;

                bool IsIntialized();
                int Transmit(const char* src, size_t size);

            private:
                static constexpr size_t BUFFER_SIZE = 2 * 1024;
                QueueHandle_t uartQueue;
                bool initialized = false;

                void Init();
        };

        static Logger logger;
        Logger();
        

};


inline bool Logger::Uart0::IsIntialized()
{
    return this->initialized;
}


NAMESPACE_END

#endif