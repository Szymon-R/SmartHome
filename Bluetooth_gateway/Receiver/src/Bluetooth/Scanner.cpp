#include "Scanner.hpp"

using namespace Bluetooth;

Scanner::Scanner()
{
    this->pBLEScan = BLEDevice::getScan();
    if (!BLEDevice::getInitialized())
    {
        BLEDevice::init("");
    }
}

bool Scanner::Scan(const ScanConfig& config)
{
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


