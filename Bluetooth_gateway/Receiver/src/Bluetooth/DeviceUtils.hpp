#include "Utils.hpp"
#include "Device.hpp"
#include <BLEDevice.h>

#ifndef _DEVICE_UTILS_HPP
#define _DEVICE_UTILS_HPP

using namespace Bluetooth;

class DeviceUtils
{
    static BLEAdvertisedDevice* FindDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name);
};

#endif