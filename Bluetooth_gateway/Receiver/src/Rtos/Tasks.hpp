/*#include "../Bluetooth/Utils.hpp"
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




class Read
{
    public:
        Read(Bluetooth::Device& dev, BLEAdvertisedDevice& scannedDev);
        virtual void Execute(const int priority = 3, const int stackSize = 100000) = 0;
        Status GetLastStatus();
        QueueHandle_t statusQueue;
        
    protected:
        static constexpr int QUEUE_SIZE = 10;
        Status lastStatus;
        void InsertStatus(Status status);
        Bluetooth::Device& dev;
        BLEClient* client = nullptr;
        TaskHandle_t *taskHandle = nullptr;
        BLEAdvertisedDevice& scannedDev;
};


class ReadOnce : public Read
{
    public:
        using Read::Read;
        void Init(Bluetooth::Service* service, Bluetooth::Characteristic* characteristic);
        void Execute(const int priority = 3, const int stackSize = 100000) override;
        ~ReadOnce();

    private:
        static void Run(void * ownedObject);
        Bluetooth::Service* service = nullptr;
        Bluetooth::Characteristic* characteristic = nullptr;
};

NAMESPACE_END

# endif*/