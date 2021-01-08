#include "DeviceList.hpp"

// Device info might be stored in xml, json or any other format

using namespace Bluetooth;

Device Devices::temperatureSensor1
{
    "Temperature Sensor 1",
    {
    Service
        {"Service1",
         "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
            {
            Characteristic
                {"Characteristic1",
                 "beb5483e-36e1-4688-b7f5-ea07361b26a8"
                }
            }
        }
    }
};


