
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>

#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "src/Utils/Logger.hpp"
#include "src/Bluetooth/Device.hpp"
#include "src/Bluetooth/Scanner.hpp"
#include "src/Bluetooth/DeviceList.hpp"
#include "src/Rtos/TaskRead.hpp"
#include "src/Rtos/Scheduler.hpp"
#include "src/Rtos/HttpHandler.hpp"
#include "src/Network/Json.hpp"
#include "src/Drivers/Timer.hpp"
#include "src/Network/JsonBuilder.hpp"



void setup()
{
  Utils::Logger::GetInstance().Initialize();
}

void loop() 
{  
  /*  Rtos::Scheduler scheduler;
    scheduler.Execute(1, 5000);
    while(true)
    {
        vTaskDelay(1000);
        LOG_LOW(".");
    }*/
   Rtos::HttpHandler httpHandler("http://192.168.1.2:1880/update-sensor");
   LOG_HIGH("Starting program\n\r");

    Bluetooth::Scanner reader;
    reader.Scan();
    while (!reader.IsScanReady()); 
    LOG_LOW("Scan completed\r\n");
   // httpHandler.Execute();
    

   /* while(1)
    {
        //httpHandler.~HttpHandler();
        LOG_LOW("Starting scan\r\n");

        reader.Scan();
        while (!reader.IsScanReady()); 
        LOG_LOW("Scan completed\r\n");

        std::vector<BLEAdvertisedDevice> scannedDevices = reader.GetDetectedDevices();
        BLEAdvertisedDevice* scannedDevice = reader.GetDeviceByName(scannedDevices, Bluetooth::Devices::temperatureSensor1.deviceName);
        if (scannedDevice)
        {
            Rtos::ReadAll readAll{Bluetooth::Devices::temperatureSensor1, *scannedDevice};
            LOG_LOW("Device found: ", Bluetooth::Devices::temperatureSensor1.deviceName, "\n\r");
            readAll.Execute();
            while(1)
            {
                auto status = readAll.GetLastStatus();
                if (status == Rtos::Status::VALUE_READ)
                {
                    LOG_LOW("Status: value read!\r\n");
                    auto dev = Network::JsonBuilder::Create(Bluetooth::Devices::temperatureSensor1);
                    auto parsed = Network::JsonBuilder::Parse(dev);
                    LOG_HIGH(parsed, "\n\r");
                    //httpHandler.InsertData(parsed);
                    break;
                }
                else if (status == Rtos::Status::TIMEOUT)
                {
                    LOG_LOW("Status: timeout!\r\n");
                    auto dev = Network::JsonBuilder::Create(Bluetooth::Devices::temperatureSensor1);
                    auto parsed = Network::JsonBuilder::Parse(dev);
                    //httpHandler.InsertData(parsed);
                    break;
                }
                else if(status == Rtos::Status::NO_STATUS)
                {

                }
                else
                {
                    LOG_LOW("Unknown reaction. Status: ",static_cast<int>(status),"\r\n");
                    break;
                }
            }
        }
        else
        {
            LOG_LOW("Device not found\r\n");
           // httpHandler.InsertData("0");
        }
    }
*/
}
