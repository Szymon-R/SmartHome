#include "DeviceUtils.hpp"

using namespace Bluetooth;

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

bool DeviceUtils::HasAnyWrite(Bluetooth::Device& dev)
{
    auto all = dev.GetAll();
    for (auto& pair : all)
    {
        for (auto chara : pair.second)
        {
            if (chara->GetMode() | Bluetooth::Mode::WRITE)
            {
                return true;
            }
        }
    }
}

bool DeviceUtils::HasReadOnly(Bluetooth::Device& dev)
{
    auto all = dev.GetAll();
    for (auto& pair : all)
    {
        for (auto chara : pair.second)
        {
            if (!(chara->GetMode() & Bluetooth::Mode::READ))
            {
                return true;
            }
        }
    }
}