#include <BLEDevice.h>
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
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "src/Bluetooth/Logger.hpp"
#include "src/Bluetooth/Device.hpp"
#include "src/Bluetooth/Scanner.hpp"
#include "src/Bluetooth/DeviceList.hpp"

void setup()
{
 //   Bluetooth::Logger::GetInstance().Initialize();
   // Bluetooth::Scanner reader;
   // LOG_LOW("Starting scan\n");
    //reader.Scan();
   // while (!reader.IsScanReady());    
    //LOG_LOW("End of the setup\n");
}

void loop() 
{
  //  LOG_LOW(global::tempSensor1.deviceName);

 /*   if (Reader.ReadData(temperatureSensor1) == Status:::READ_SUCCESS)
    {
        Notify(temperatureSensor1);
    }*/

   // delay(1000);
} 
