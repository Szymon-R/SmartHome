
#ifndef _TASK_READ_HPP
#define _TASK_READ_HPP

#include "../Utils/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include "../Utils/Logger.hpp"

#include "Task.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"

#include <vector>
#include <memory>
#include <BLEDevice.h>

NAMESPACE_START(Rtos)



class Read : public Task
{
    public:
        Read(Bluetooth::Device& dev, BLEAdvertisedDevice& scannedDev);
        
    protected:
        Bluetooth::Device& dev;
        BLEClient* client = nullptr;
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

# endif