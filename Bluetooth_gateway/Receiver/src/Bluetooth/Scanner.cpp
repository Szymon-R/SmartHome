#include "Scanner.hpp"

using namespace Bluetooth;

Scanner::Scanner()
{
    this->pBLEScan = BLEDevice::getScan();
}

bool Scanner::Scan(const ScanConfig& config)
{
    this->scanReady = false;
    if (!this->radioGuard.Acquire(Utils::Protocol::BLUETOOTH))
    {
        LOG_LOW("Didnt acquire to scan\n\r");
        return false;
    }
    this->devices.clear();
    if (this->userCallback)
    {   
        pBLEScan->setAdvertisedDeviceCallbacks(this->userCallback.get());
    }
    else
    {
        pBLEScan->setAdvertisedDeviceCallbacks(&this->defaultCallback);
    }
    pBLEScan->setInterval(config.interval);
    pBLEScan->setWindow(config.window);
    pBLEScan->start(config.scanTime, config.extended);
    pBLEScan->setActiveScan(config.activeScan);
    this->scanReady = true;
    LOG_LOW("Scan release\n\r");

    this->radioGuard.Release(Utils::Protocol::BLUETOOTH);
    return true;
}

void Scanner::InsertCallback(std::unique_ptr<BLEAdvertisedDeviceCallbacks>&& callback)
{
    if (this->userCallback)
    {
        userCallback.reset();
        userCallback = std::move(callback);
    }
}

bool Scanner::IsScanReady()
{
    return this->scanReady;
}

BLEAdvertisedDevice* Scanner::GetDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name)
{
    BLEAdvertisedDevice* out = nullptr;
   // LOG_LOW("Devices found: \n\r");
    for (auto& device : devices)
    {
        //LOG_LOW("\t- ", device.getName(), "\n\r");
        if (device.getName() == name)
        {
            out = const_cast<BLEAdvertisedDevice*>(&device);
            break;
        }
    }
    return out;
}

std::vector<BLEAdvertisedDevice> Scanner::GetDetectedDevices()
{
    while (!this->scanReady);
    return this->devices;
}


Scanner::ScanCallback::ScanCallback(std::vector<BLEAdvertisedDevice>& devices) : devices(devices)
{
}

void Scanner::ScanCallback::onResult(BLEAdvertisedDevice advertisedDevice)
{
   /* auto IsDeviceValid = [&]()
    {
        if (advertisedDevice.getName() == "")
        {
            return false;
        }
        else
        {
            auto iter = std::find_if(Devices::devices.begin(), Devices::devices.end(), [&](Device& device) -> bool
            {
                LOG_LOW("Device: ", device.deviceName, "\n\r");
                LOG_LOW("Advert device: ",advertisedDevice.getName(), "\n\r");
                return device.deviceName == advertisedDevice.getName();
            });
            if (iter == Devices::devices.end())
            {
                return false;
            }
        }
    };
    if (IsDeviceValid())
    {
        LOG_LOW("Adding device: ",advertisedDevice.getName(), "\n\r");
        this->devices.emplace_back(advertisedDevice);
    }*/

    LOG_LOW("Checking device: ", advertisedDevice.getName(), "\n\r");
    for (auto& device : Devices::devices)
    {
        if(advertisedDevice.getName() == device.deviceName)
        {
            LOG_LOW("Device found\n\r");
           // this->devices.emplace_back(advertisedDevice);
            break;
        }
    }
}


