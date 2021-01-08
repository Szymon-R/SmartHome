#include "../Bluetooth/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include "../Bluetooth/Logger.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"

#include <vector>
#include <memory>
#include <BLEDevice.h>

#ifndef _TASKS_HPP
#define _TASKS_HPP

NAMESPACE_START(Rtos)

/*
enum class Status
{
    static constexpr int OK = 1;
    static constexpr int TIMEOUT = 2;
    static constexpr int VALUE_READ = 3;
    static constexpr int DEVICE_NOT_FOUND = 4;
    static constexpr int SERVICE_NOT_FOUND = 5;
    static constexpr int CHARACTERISTIC_NOT_FOUND = 6;
};*/

enum class Status : int
{
    NO_STATUS,
    UPDATE_ERROR,
    OK,
    TIMEOUT,
    VALUE_READ,
    DEVICE_NOT_FOUND,
    SERVICE_NOT_FOUND,
    NOT_CONNECTED,
    CHARACTERISTIC_NOT_FOUND
};

/*
class Read
{
    Read(Bluetooth::Device& dev, BLEAdvertisedDevice* scannedDev);
};
*/

class ReadOnce
{
    public:
        ReadOnce(Bluetooth::Device& dev, BLEAdvertisedDevice& scannedDev);
        QueueHandle_t statusQueue;
        void Init(Bluetooth::Service* service, Bluetooth::Characteristic* characteristic);
        void Execute(const int priority = 3, const int stackSize = 100000);
        Status GetLastStatus();
        ~ReadOnce();

    private:
        static constexpr int QUEUE_SIZE = 10;
        Status lastStatus;
        static void Run(void * ownedObject);
        void InsertStatus(Status status);
        Bluetooth::Device& dev;
        BLEClient* client = nullptr;
        TaskHandle_t *taskHandle = nullptr;
        BLEAdvertisedDevice& scannedDev;
        Bluetooth::Service* service = nullptr;
        Bluetooth::Characteristic* characteristic = nullptr;
};

NAMESPACE_END

# endif