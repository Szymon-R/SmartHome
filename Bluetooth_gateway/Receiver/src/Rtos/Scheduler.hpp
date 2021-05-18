#ifndef _SCHEDULER_HPP
#define _SCHEDULER_HPP

#include "../Utils/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include "../Bluetooth/DeviceList.hpp"
#include "../Bluetooth/DeviceUtils.hpp"
#include "../Utils/Logger.hpp"
#include "../Utils/RadioGuard.hpp"

#include "HttpHandler.hpp"
#include "TaskRead.hpp"
#include "../Bluetooth/Scanner.hpp"

#include "freertos/queue.h"
#include "freertos/task.h"

#include <vector>
#include <memory>

NAMESPACE_START(Rtos)

class Scheduler : public Task
{
public:
    Scheduler();
    ~Scheduler();
    void Init();
    void Execute(const int priority = 3, const int stackSize = 10000) override;

private:
    static void Run(void * ownedObject);
    void RunBluetoothTasks(std::vector<BLEAdvertisedDevice>& scanned);
    bool MonitorTasks();
    Bluetooth::Scanner scanner;
    std::vector<Bluetooth::Device>& bluetoothDevices;
    std::vector<std::unique_ptr<Rtos::Task>> tasks;
};

NAMESPACE_END

#endif