#ifndef _DEVICE_UTILS_HPP
#define _DEVICE_UTILS_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include "Device.hpp"
#include <BLEDevice.h>

NAMESPACE_START(Bluetooth)

class DeviceUtils
{
    public:
        static BLEAdvertisedDevice* FindDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name);
        static bool HasAnyWrite(Bluetooth::Device& dev);
        static bool HasOnlyRead(Bluetooth::Device& dev);
};


NAMESPACE_END

#endif