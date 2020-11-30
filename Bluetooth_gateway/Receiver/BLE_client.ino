#include <BLEDevice.h>
#include "src/Bluetooth/Reader.hpp"
#include "driver/uart.h"
#include "driver/uart_select.h"
//#include "BLEScan.h"

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include<string>
#include<stdio.h>
#include<stdlib.h>


const int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;

void setup() {

  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .rx_flow_ctrl_thresh = 122,
  };


  // Setup UART buffered IO with event queue

    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(uart_port_t::UART_NUM_0, uart_buffer_size, \
                                        uart_buffer_size, 10, &uart_queue, 0));
                                          
    ESP_ERROR_CHECK(uart_param_config(uart_port_t::UART_NUM_0, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 18, 19));


} // End of setup.

void loop() 
{

    char* test_str = "This is a test string.\n";
    uart_write_bytes(uart_port_t::UART_NUM_0, (const char*)test_str, strlen(test_str));
    delay(1000);
   // ESP_ERROR_CHECK(uart_wait_tx_done(uart_port_t::UART_NUM_0, 1000)); // wait timeout is 100 RTOS ticks (TickType_t)
} 
