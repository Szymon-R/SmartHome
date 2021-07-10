/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID_1        "ffa8fabe-24be-40b4-b234-01c08ce2b3eb"
#define CHARACTERISTIC_UUID_1_1 "09836e84-a18c-4692-a361-8e7e535f8874"
#define CHARACTERISTIC_UUID_1_2 "472f697e-b7f5-41d5-908e-bd2a74020b29"

#define SERVICE_UUID_2        "f7c09b94-64d6-4d90-8d7c-83b92c5c0f30"
#define CHARACTERISTIC_UUID_2_1 "83e577b7-931a-44d0-81eb-1b1910bfd349"


struct OnDisconnect : public BLEServerCallbacks
{
    void onDisconnect(BLEServer* pServer) override
    {
        BLEDevice::startAdvertising();
    }
};


BLECharacteristic *pCharacteristic1_1;
BLECharacteristic *pCharacteristic1_2;
BLECharacteristic *pCharacteristic2_1;
BLEServer *pServer;
OnDisconnect onDisc;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Water sensor");
  pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID_1);
  pServer->setCallbacks(&onDisc);

  pCharacteristic1_1 = pService->createCharacteristic(CHARACTERISTIC_UUID_1_1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic1_1-> addDescriptor (new BLE2902 ());
  pCharacteristic1_1->setValue("Hello World says Neil");

  pCharacteristic1_2 = pService->createCharacteristic(CHARACTERISTIC_UUID_1_2, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic1_2-> addDescriptor (new BLE2902 ());
  pCharacteristic1_2->setValue("Hello World says Neil");
  pService->start();

  BLEService *pService2 = pServer->createService(SERVICE_UUID_2);
  pCharacteristic2_1 = pService->createCharacteristic(CHARACTERISTIC_UUID_2_1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic2_1-> addDescriptor (new BLE2902 ());
  pCharacteristic2_1->setValue("Hello World says Neil");
  pService2->start();

  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID_1);
  pAdvertising->addServiceUUID(SERVICE_UUID_2);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() 
{
  static int counter = 200;
  char buffer [33];
  itoa (counter,buffer,10);
  pCharacteristic1_1->setValue(buffer);
  pCharacteristic1_2->setValue(buffer);
  pCharacteristic2_1->setValue(buffer);
  ++counter;
  if (counter > 299)
  {
      counter = 299;
  }
  delay(2000);
}
