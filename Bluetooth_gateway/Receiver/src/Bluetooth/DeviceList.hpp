#include "Utils.hpp"
#include "Device.hpp"

#ifndef _DEVICE_LIST_HPP
#define _DEVICE_LIST_HPP

using namespace Bluetooth;

class Devices
{
    public:
        static Device temperatureSensor1;
        static Device preassureSensor;
};

#endif


/*
void ReadOnce(device d)
{
    if (device.connect() == false)
        statusQueue = NOT_CONNECTED;
        return;
    else
    {
        device.Read();

    }
    

}*/