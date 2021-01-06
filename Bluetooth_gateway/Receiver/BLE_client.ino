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


#include "src/Rtos/Tasks.hpp"

Bluetooth::Scanner reader;
std::string serviceName = "Service1";
std::string characteristicName = "Char1";
void setup()
{
    //Bluetooth::Logger::GetInstance().Initialize();
    
  //  LOG_LOW("Starting scan\n");
   // reader.Scan();
   // while (!reader.IsScanReady()); 
  //  LOG_LOW("End of the setup\r\n");
}

void loop() 
{
    while(1)
    {
        
    }
    LOG_LOW("Starting the loop\n\r");
    std::vector<BLEAdvertisedDevice> scannedDevices = reader.GetDetectedDevices();
    BLEAdvertisedDevice* scannedDevice = reader.GetDeviceByName(scannedDevices, Devices::temperatureSensor1.deviceName);
    Rtos::ReadOnce readOnce{Devices::temperatureSensor1, *scannedDevice};
    readOnce.Init(Devices::temperatureSensor1[serviceName], (*Devices::temperatureSensor1[serviceName])[characteristicName]);
    readOnce.Execute();
    while (readOnce.GetLastStatus() != Rtos::Status::OK)
    {
        LOG_LOW("Waiting for status\r\n");
        delay(1000);

    }
    LOG_LOW("Status received\r\n");
    readOnce.~ReadOnce();
    if (scannedDevice)
    {
        LOG_LOW("Device: ", Devices::temperatureSensor1.deviceName, " found\n\r");
        Rtos::ReadOnce readOnce{Devices::temperatureSensor1, *scannedDevice};
        readOnce.Init(Devices::temperatureSensor1[serviceName], (*Devices::temperatureSensor1[serviceName])[characteristicName]);
        readOnce.Execute();
        while (readOnce.GetLastStatus() != Rtos::Status::OK)
        {
            LOG_LOW("Waiting for status\r\n");
            delay(1000);

        }
        LOG_LOW("Status received\r\n");
        readOnce.~ReadOnce();
    }
    


    while(1)
    {

    }


    delay(1000);
} 
