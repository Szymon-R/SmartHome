#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "Utils.hpp"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "freertos/queue.h"
#include <type_traits>
#include <string>
#include <sstream>

NAMESPACE_START(Bluetooth)




#ifdef LOG_LEVEL_LOW
    #define LOG_LEVEL_MEDIUM
    #define LOG_LEVEL_HIGH
    #define LOG_LOW(...) Bluetooth::Logger::GetInstance().Transmit(__VA_ARGS__)
#else
    #define LOG_LOW(...)
#endif

#ifdef LOG_LEVEL_MEDIUM
#define LOG_LEVEL_HIGH
    #define LOG_MEDIUM(...) Bluetooth::Logger::GetInstance().Transmit(__VA_ARGS__)
#else
    #define LOG_MEDIUM(...)
#endif

#ifdef LOG_LEVEL_HIGH
    #define LOG_HIGH(...) Bluetooth::Logger::GetInstance().Transmit(__VA_ARGS__)
#else
    #define LOG_HIGH(...)
#endif

class Logger
{
    public:
        static Logger& GetInstance();
        template<typename T, typename... Args>
        bool Transmit(T&& first, Args&&... args);
        void Initialize();
        Logger(Logger const&) = delete;
        void operator=(Logger const&)  = delete;

    private:
        Logger();
        class Uart0
        {
            public:
                uart_config_t uartConfig = 
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
                void Init();

            private:
                static constexpr size_t BUFFER_SIZE = 2 * 1024;
                QueueHandle_t uartQueue;
                bool initialized = false;                
        };
       
        Uart0 uart0;        
        bool Transmit();

        // Intigers
        template<typename T,
            typename std::enable_if_t<std::is_integral_v<typename std::remove_reference_t<T>>, bool> = false>
        bool TransmitImpl(T&& arg)
        {
            std::stringstream s;
            s << arg;
            return TransmitImpl(s.str());
        }
        // strings
        template<typename T,
            typename std::enable_if_t<std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, std::string>, bool> = false>
        bool TransmitImpl(T&& arg)
        {
            return TransmitImpl(arg.c_str());
        }

        // floats
        template<typename T,
            typename std::enable_if_t<std::is_floating_point_v<typename std::remove_reference_t<T>>, bool> = false>
        bool TransmitImpl(T&& arg)
        {
            std::stringstream s;
            s << arg;
            return TransmitImpl(s.str());
        }

        // const char*
        bool TransmitImpl(const char* text)
        {
            uart0.Transmit(text, strlen(text));
        }
};

inline bool Logger::Uart0::IsIntialized()
{
    return this->initialized;
}

template<typename T, typename... Args>
bool Logger::Transmit(T&& first, Args&&... args) 
{   
    return (this->TransmitImpl(first) & this->Transmit(args...));
}

NAMESPACE_END

#endif