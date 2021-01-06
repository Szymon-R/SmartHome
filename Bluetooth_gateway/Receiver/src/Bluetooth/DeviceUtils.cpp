#include "DeviceUtils.hpp"

BLEAdvertisedDevice* DeviceUtils::FindDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name)
{
    BLEAdvertisedDevice* output = nullptr;
    for (auto& device : devices)
    {
        if (device.getName() == name)
        {
            output = &device;
            break;
        }
    }
    return output;
}