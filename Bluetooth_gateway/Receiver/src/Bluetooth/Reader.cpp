#include "Reader.hpp"



using namespace Bluetooth;

Reader::Reader()
{
    this->pBLEScan = BLEDevice::getScan();
    if (!BLEDevice::getInitialized())
    {
        BLEDevice::init("");
    }
}

bool Reader::Scan(const ScanConfig& config)
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
}

void Reader::InsertCallback(std::unique_ptr<BLEAdvertisedDeviceCallbacks>&& callback)
{
    if (this->userCallback)
    {
        userCallback.reset();
        userCallback = std::move(callback);
    }
}

std::vector<BLEAdvertisedDevice> Reader::GetDetectedDevices()
{
    while (!this->scanReady);
    return this->devices;
}

Reader::ScanCallback::ScanCallback(std::vector<BLEAdvertisedDevice>& devices) : devices(devices)
{

}

void Reader::ScanCallback::onResult(BLEAdvertisedDevice advertisedDevice)
{
    this->devices.emplace_back(advertisedDevice);
}


