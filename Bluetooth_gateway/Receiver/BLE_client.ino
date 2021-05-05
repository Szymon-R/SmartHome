
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
#include "src/Rtos/HttpPost.hpp"
#include "src/Network/Json.hpp"
#include "src/Drivers/Timer.hpp"
#include "src/Network/JsonBuilder.hpp"



void setup()
{
  Utils::Logger::GetInstance().Initialize();
}

void loop() 
{  
    Rtos::HttpPost httpPost("http://192.168.1.2:1880/update-sensor");
    LOG_LOW("Scan completed\r\n");
    httpPost.Execute();
    const auto jsons =  Network::JsonBuilder::Create(Bluetooth::Devices::devices);
    httpPost.InsertData(Network::JsonParser::Parse(jsons));
}
