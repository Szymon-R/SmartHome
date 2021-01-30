#ifndef _DEVICE_UTILS_HPP
#define _DEVICE_UTILS_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include <BLEDevice.h>

NAMESPACE_START(Bluetooth)

class DeviceUtils
{
    static BLEAdvertisedDevice* FindDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name);
};

NAMESPACE_END

#endif