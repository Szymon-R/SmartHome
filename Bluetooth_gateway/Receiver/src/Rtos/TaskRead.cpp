#include "TaskRead.hpp"


using namespace Rtos;

Read::Read(Bluetooth::Device& dev, BLEAdvertisedDevice& scannedDev) : dev(dev), scannedDev(scannedDev)
{
    this->statusQueue =  xQueueCreate(ReadOnce::QUEUE_SIZE, sizeof(int*));
}

void ReadOnce::Init(Bluetooth::Service* service, Bluetooth::Characteristic* characteristic)
{
    this->service = service;
    this->characteristic = characteristic;
}

void ReadOnce::Execute(const int priority, const int stackSize)
{
    if (xTaskCreate(ReadOnce::Run,
                    "ReadOnce",
                    stackSize,
                    (void*)this,
                    priority,
                    this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task: ReadOnce\r\n");
    }
}

void ReadOnce::Run(void* ownedObject)
{
    while(1)
    {
        ReadOnce* caller = reinterpret_cast<ReadOnce*>(ownedObject);
        caller->client = BLEDevice::createClient();
        if (!caller->client)
        {
            LOG_HIGH("Couldn't create client!\n\r");
            vTaskSuspend(NULL);
        }

        caller->client->connect(&caller->scannedDev);
        vTaskDelay(1000);
       
        if (caller->client->isConnected())
        {
            LOG_LOW("Client connected\n\r");
            BLERemoteService* pRemoteService = caller->client->getService(caller->service->serviceCode.c_str());
            if (pRemoteService == nullptr) 
            {
                LOG_MEDIUM("Couldn't read service: ", caller->service->serviceName, "\n\r");
                caller->InsertStatus(Status::SERVICE_NOT_FOUND);
            }
            else
            {
                // Obtain a reference to the characteristic in the service of the remote BLE server.
                BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(caller->characteristic->charCode.c_str());
                if (pRemoteCharacteristic == nullptr) 
                {
                    LOG_MEDIUM("Couldn't read characteristic: ", caller->characteristic->charName, "\n\r");
                    caller->InsertStatus(Status::CHARACTERISTIC_NOT_FOUND);
                }
                else
                {
                    std::string value = pRemoteCharacteristic->readValue();
                    LOG_LOW("Read value: ", value, "\n\r");
                    caller->dev.InsertValue(caller->characteristic, value);
                    caller->InsertStatus(Status::VALUE_READ);
                }
            }
            caller->client->disconnect();
        }
        else
        {
            LOG_MEDIUM("Client not connected\n\r");
            caller->InsertStatus(Status::NOT_CONNECTED);
        }
        vTaskSuspend(NULL);
    }
}


ReadOnce::~ReadOnce()
{

}


void ReadAll::Execute(const int priority, const int stackSize)
{
    if (xTaskCreate(ReadAll::Run,
                    "ReadAll",
                    stackSize,
                    (void*)this,
                    priority,
                    this->taskHandle
                    ) != pdPASS )
    {
        LOG_HIGH("Couldn't create task: ReadAll\r\n");
    }
}

void ReadAll::Run(void* ownedObject)
{

    static constexpr int IDLE = 0;
    static constexpr int INIT = 1;
    static constexpr int RUN = 2;
    static constexpr int EXIT = 3;

    ReadAll* caller = reinterpret_cast<ReadAll*>(ownedObject);
    int state = INIT;
    while (1)
    {

        switch (state)
        {
            case IDLE:
            {

            }
            break;

            case INIT:
            {
                if (!caller->radioGuard.Acquire(Utils::Protocol::BLUETOOTH))
                {
                    LOG_HIGH("Couldn't acquire bluetooth radio for ReadAll class.\n\r");
                    caller->InsertStatus(Status::NOT_CONNECTED);
                }
                else
                {
                    state = RUN;
                    caller->client = BLEDevice::createClient();
                    if (!caller->client)
                    {
                        LOG_HIGH("Couldn't create client!\n\r");
                        vTaskSuspend(NULL);
                    }
                }

            }
            break;

            case RUN:
            {
                caller->client->connect(&caller->scannedDev);
                vTaskDelay(500);
                if (caller->client->isConnected())
                {
                    LOG_LOW("Client connected\n\r");
                    const auto& services = caller->dev.GetServices();
                    for (auto& service : services)
                    {
                        BLERemoteService* pRemoteService = caller->client->getService(service.serviceCode.c_str());
                        if (pRemoteService == nullptr)
                        {
                            LOG_MEDIUM("Couldn't find service: ", service.serviceName);
                        }
                        else
                        {
                            const auto& characteristics = service.GetCharacteristics();
                            for (auto& characteristic : characteristics)
                            {
                                BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(characteristic.charCode.c_str());
                                if (pRemoteCharacteristic == nullptr) 
                                {
                                    LOG_MEDIUM("Couldn't read characteristic: ", characteristic.charName, "\n\r");
                                }
                                else
                                {
                                    std::string value = pRemoteCharacteristic->readValue();
                                    LOG_LOW("Read value: ", value, "\n\r");
                                    caller->dev.InsertValue(const_cast<Bluetooth::Characteristic*>(&characteristic), value);
                                    caller->InsertStatus(Status::VALUE_READ);
                                }
                            }
                            
                        }
                    }
                    caller->client->disconnect();
                }
                else
                {
                    LOG_MEDIUM("Client not connected\n\r");
                    caller->InsertStatus(Status::NOT_CONNECTED);
                }
                state = EXIT;
            }
            break;

            case EXIT:
            {
                if (!caller->radioGuard.Release(Utils::Protocol::BLUETOOTH))
                {
                    LOG_HIGH("Couldn't acquire bluetooth radio for ReadAll class.\n\r");
                }
                state = IDLE;
            }
            break;

            default:
            break;
        }
    }
} 


ReadAll::~ReadAll()
{
    LOG_LOW("There are: ", ReadAll::QUEUE_SIZE - uxQueueSpacesAvailable(this->statusQueue), " elements on queue\r\n");
    while (uxQueueSpacesAvailable(this->statusQueue) != ReadAll::QUEUE_SIZE)
    {
        LOG_LOW("Deleting item\r\n");
        int* buffer = nullptr;
        xQueueReceive(this->statusQueue, &buffer, 0);
        if (!buffer)
        {
            LOG_HIGH("Empty element received from queue\r\n");
        }
        else
        {
            delete buffer;
            LOG_LOW("Item deleted\r\n");
        }
    }
    if (this->taskHandle)
    {
        vTaskDelete(this->taskHandle);
    }
}