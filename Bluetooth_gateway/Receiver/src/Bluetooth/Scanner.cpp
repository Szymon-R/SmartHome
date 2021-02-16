#include "Scanner.hpp"

using namespace Bluetooth;

Scanner::Scanner()
{
    this->pBLEScan = BLEDevice::getScan();
}

bool Scanner::Scan(const ScanConfig& config)
{
    if (!BLEDevice::getInitialized())
    {
        BLEDevice::init("");
    }
    this->scanReady = false;
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
    pBLEScan->setActiveScan(config.activeScan);
    pBLEScan->start(config.scanTime, config.extended);
    this->scanReady = true;
    if (BLEDevice::getInitialized())
    {
        BLEDevice::deinit(false);
    }
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
    LOG_LOW("Devices found: \n\r");
    for (auto& device : devices)
    {
        LOG_LOW("\t- ", device.getName(), "\n\r");
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
    this->devices.emplace_back(advertisedDevice);
}


