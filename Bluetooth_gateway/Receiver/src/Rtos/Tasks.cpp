#include "Tasks.hpp"

using namespace Rtos;

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");

ReadOnce::ReadOnce(Bluetooth::Device& dev, BLEAdvertisedDevice& scannedDev) : dev(dev), scannedDev(scannedDev)
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
        LOG_HIGH("Couldn't create task\r\n");
    }
}

void ReadOnce::Run(void* ownedObject)
{
    while(1)
    {
        //heap_caps_check_integrity();
        ReadOnce* caller = reinterpret_cast<ReadOnce*>(ownedObject);
        caller->InsertStatus(Status::VALUE_READ);
        caller->client = BLEDevice::createClient();
        if (!caller->client)
        {
            LOG_HIGH("Couldn't create client!\n\r");
            vTaskSuspend(NULL);
        }

        caller->client->connect(&caller->scannedDev);
        delay(500); 
        if (caller->client->isConnected())
        {
            BLERemoteService* pRemoteService = caller->client->getService(caller->service->serviceCode.c_str());
           //BLERemoteService* pRemoteService = caller->client->getService(serviceUUID);
          /*  if (pRemoteService == nullptr) 
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
                    caller->dev.InsertValue(caller->service, caller->characteristic, value);
                    caller->InsertStatus(Status::VALUE_READ);
                }
            }*/
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

void ReadOnce::InsertStatus(Status status)
{
    int* a = new int;
    *a = static_cast<int>(status);
    xQueueSend(this->statusQueue, (void*)&a, 0);
}

Status ReadOnce::GetLastStatus()
{
    int* buffer = nullptr;
    if (xQueueReceive(this->statusQueue, &buffer, 0))
    {
        int a = *buffer;
        delete buffer;
        return static_cast<Status>(a);
    }    
}

ReadOnce::~ReadOnce()
{
    LOG_LOW("There are: ", ReadOnce::QUEUE_SIZE - uxQueueSpacesAvailable(this->statusQueue), " elements on queue\r\n");
    while (uxQueueSpacesAvailable(this->statusQueue) != ReadOnce::QUEUE_SIZE)
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