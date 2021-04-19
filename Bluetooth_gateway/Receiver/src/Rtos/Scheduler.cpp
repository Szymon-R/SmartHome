#include "Scheduler.hpp"

using namespace Rtos;

Scheduler::Scheduler() :
    httpHandler(HttpHandler{"http://192.168.1.2:1880/update-sensor"}),
    scanner(Bluetooth::Scanner{}),
    bluetoothDevices(Bluetooth::Devices::devices)
{

}

void Scheduler::Init()
{
    for (auto& device : Bluetooth::Devices::devices)
    {
        bluetoothDevices.push_back(device);
    }

}

void Scheduler::Execute(const int priority, const int stackSize)
{
    if (xTaskCreate(Scheduler::Run,
                    "Scheduler",
                    stackSize,
                    (void*)this,
                    priority,
                    this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task: Scheduler\r\n");
    }
}


void Scheduler::Run(void * ownedObject)
{
    auto& caller = *(static_cast<Scheduler*>(ownedObject));
    caller.httpHandler.Execute();

    while(true)
    {
        // Scanning
        caller.scanner.Scan();
        while (!caller.scanner.IsScanReady()); 
        LOG_LOW("Scan completed\r\n");

        auto detectedDevice = caller.scanner.GetDetectedDevices();
        LOG_LOW("Size: ", detectedDevice.size(), "\n\r");
        for (auto& scan: detectedDevice) 
        {
           // LOG_LOW(scan.getName(), "\n\r");
        }
        caller.RunBluetoothTasks(detectedDevice);
        vTaskDelay(1000);
    }
}

void Scheduler::RunBluetoothTasks(std::vector<BLEAdvertisedDevice>& scanned)
{
    for (auto& scan: scanned) 
    {
        auto it = std::find_if(this->bluetoothDevices.begin(), this->bluetoothDevices.end(), 
                    [&](Bluetooth::Device& ble) -> bool {
                    return scan.getName() == ble.deviceName; });
        if (it != bluetoothDevices.end())
        {
            LOG_LOW("Device found!\r\n");
            this->tasks.emplace_back(Rtos::ReadAll{*it, scan});
            tasks.back().Execute();
        }
    }
}