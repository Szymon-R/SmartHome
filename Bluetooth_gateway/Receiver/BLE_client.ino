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

#include "src/Utils/Logger.hpp"
#include "src/Bluetooth/Device.hpp"
#include "src/Bluetooth/Scanner.hpp"
#include "src/Bluetooth/DeviceList.hpp"
#include "src/Rtos/Task.hpp"
#include "src/Rtos/HttpHandler.hpp"
#include "src/Network/Json.hpp"
#include "src/Drivers/Timer.hpp"
#include "src/Network/JsonBuilder.hpp"

void setup()
{


}

void loop() 
{
    Utils::Logger::GetInstance().Initialize();
    /*
    Bluetooth::Scanner reader;
    std::string serviceName = "Service1";
    std::string characteristicName = "Characteristic1";

    Bluetooth::Logger::GetInstance().Initialize();
    LOG_LOW("Starting scan\r\n");
    reader.Scan();
    while (!reader.IsScanReady()); 
    LOG_LOW("End of the setup\r\n");

    LOG_LOW("Starting the loop\n\r");
    std::vector<BLEAdvertisedDevice> scannedDevices = reader.GetDetectedDevices();
    BLEAdvertisedDevice* scannedDevice = reader.GetDeviceByName(scannedDevices, Devices::temperatureSensor1.deviceName);
    if (scannedDevice)
    {
        LOG_LOW("Device: ", Devices::temperatureSensor1.deviceName, " found\n\r");
        Rtos::ReadOnce readOnce{Devices::temperatureSensor1, *scannedDevice};
        readOnce.Init(Devices::temperatureSensor1[serviceName], Devices::temperatureSensor1[serviceName]->operator[](characteristicName));
        readOnce.Execute();

        while (readOnce.GetLastStatus() != Rtos::Status::VALUE_READ)
        {
            (*Devices::temperatureSensor1[serviceName])[characteristicName]->GetValue().value;
            LOG_LOW("Waiting for status\r\n");
            vTaskDelay(1000);
        }
        LOG_LOW("Value: ", (*Devices::temperatureSensor1[serviceName])[characteristicName]->GetValue().value, "\n\r");
    }
    else
    {
        LOG_LOW("Didn't find device\n\r");
    }
    */
/*
    Utils::Logger::GetInstance().Initialize();
    Rtos::HttpHandler httpHandler;
    httpHandler.Execute();
    while (httpHandler.GetLastStatus() != Rtos::Status::CONNECTED);
    LOG_LOW("Success\n\r");
    httpHandler.~HttpHandler();
    while(1)
    {

    }*/

   /* 
    Rtos::HttpHandler httpHandler("http://192.168.1.2:1880/update-sensor");
    httpHandler.Execute();
    while (httpHandler.GetLastStatus() != Rtos::Status::CONNECTED);
    LOG_LOW("Success\n\r");*/
   // httpHandler.~HttpHandler();
    auto dev = Network::JsonBuilder::Create(Bluetooth::Devices::temperatureSensor1);
    auto parsed = Network::JsonBuilder::Parse(dev);
    
    while(1)
    {
        LOG_HIGH(parsed);
        vTaskDelay(1000);
    }

}
