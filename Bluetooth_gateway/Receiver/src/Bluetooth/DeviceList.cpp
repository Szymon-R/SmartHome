#include "DeviceList.hpp"

// Device info might be stored in xml, json or any other format

using namespace Bluetooth;

Device Devices::temperatureSensor1
{
    "Temperature sensor 1",
    {
    Service
        {"Service1",
            {
            Characteristic
                {"Characteristic1"},
            Characteristic
                {"Characteristic2"}
            }
        },
        Service
            {"Service1",
                {
                Characteristic
                    {"Characteristic1"},
                Characteristic
                    {"Characteristic2"}
                }
            }
    }
};

Device Devices::preassureSensor
{
    "Preassure sensor 1",
    {
    Service
        {"Service1",
            {
            Characteristic
                {"Characteristic1"},
            Characteristic
                {"Characteristic2"}
            }
        },
    Service
        {"Service2",
            {
            Characteristic
                {"Characteristic1"},
            Characteristic
                {"Characteristic2"}
            }
        }
    }
};

