#ifndef _READER_HPP
#define _READER_HPP


#include <memory>
#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include <BLEDevice.h>

NAMESPACE_START(Bluetooth)

struct ScanConfig
{
    uint16_t interval;
    uint16_t window;
    uint32_t scanTime;
    bool activeScan;
    bool extended;
};


static constexpr ScanConfig defaultConfig = {1349U, 449U, 5U, true, false};

class Scanner
{
    public:
        Scanner();
        bool Scan(const ScanConfig& config = defaultConfig);
        void InsertCallback(std::unique_ptr<BLEAdvertisedDeviceCallbacks>&& callback);
        std::vector<BLEAdvertisedDevice> GetDetectedDevices();
        bool IsScanReady();
        BLEAdvertisedDevice* GetDeviceByName(std::vector<BLEAdvertisedDevice>& devices, const std::string& name);
        
        class ScanCallback: public BLEAdvertisedDeviceCallbacks
        {
            public:
                ScanCallback(std::vector<BLEAdvertisedDevice>& devices);
                void onResult(BLEAdvertisedDevice advertisedDevice) override;

            private:
                std::vector<BLEAdvertisedDevice>& devices;                
        };

    private:
        bool scanReady = true;
        std::vector<BLEAdvertisedDevice> devices;
        BLEScan* pBLEScan = BLEDevice::getScan();
        ScanCallback defaultCallback{devices};
        std::unique_ptr<BLEAdvertisedDeviceCallbacks> userCallback;
};



NAMESPACE_END

#endif