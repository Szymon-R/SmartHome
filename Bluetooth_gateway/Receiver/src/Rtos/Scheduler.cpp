#include "Scheduler.hpp"

using namespace Rtos;

Scheduler::Scheduler() :
    scanner(Bluetooth::Scanner{}),
    bluetoothDevices(Bluetooth::Devices::devices)
{

}

Scheduler::~Scheduler()
{
    LOG_LOW("Scheduler destructor called\r\n");
    if (this->taskHandle != nullptr)
    {
        vTaskDelete(this->taskHandle);
        this->taskHandle = nullptr;
    }
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
                    &this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task: Scheduler\r\n");
    }
}


void Scheduler::Run(void * ownedObject)
{
    auto& caller = *(static_cast<Scheduler*>(ownedObject));

    while(true)
    {
        // Scanning
        caller.scanner.Scan();
        while (!caller.scanner.IsScanReady()); 
        LOG_LOW("Scan completed\r\n");

        auto detectedDevice = caller.scanner.GetDetectedDevices();
        LOG_LOW("Number of devices found: ", detectedDevice.size(), "\n\r");

        // Run specific bluetooth tasks
        caller.RunBluetoothTasks(detectedDevice);
        vTaskDelay(200);

        // Wait until tasks are complete
        LOG_LOW("Starting waiting for tasks\n\r");
        while (!caller.MonitorTasks())
        {
            vTaskDelay(200);
        }
        LOG_LOW("All tasks completed\n\r");
        caller.InsertStatus(Rtos::Status::OK);
        vTaskSuspend(NULL);
    }
}

void Scheduler::RunBluetoothTasks(std::vector<BLEAdvertisedDevice>& scanned)
{
    // Run tasks for all detected devices
    for (auto& scan: scanned) 
    {
        auto it = std::find_if(this->bluetoothDevices.begin(), this->bluetoothDevices.end(), 
                    [&](Bluetooth::Device& ble) -> bool {
                    return scan.getName() == ble.deviceName; });
        if (it != bluetoothDevices.end())
        {
            LOG_LOW("Device: ", it->deviceName," found\r\n");
            if (Bluetooth::DeviceUtils::HasOnlyRead(*it))
            {
                this->tasks.emplace_back(std::unique_ptr<Rtos::ReadAll>(new ReadAll(*it, scan)));
            }
            this->tasks.back()->Execute();
            vTaskDelay(50);
        }
    }
}

bool Scheduler::MonitorTasks()
{
    bool tasksFinished = true;
    for (size_t i = 0; i < this->tasks.size(); ++i)
    {
        if (this->tasks[i]->GetLastStatus() != Rtos::Status::FINISHED)
        {
            tasksFinished = false;
            break;
        }
    }
    if (tasksFinished)
    {
        tasks.clear();
    }
    return (this->tasks.size() == 0U);
}