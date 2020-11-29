#include <BLEDevice.h>
#include "src/Bluetooth/Reader.hpp"
//#include "BLEScan.h"

#include<string>
#include<stdio.h>
#include<stdlib.h>

#define uS_TO_MS_FACTOR 1000

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");


Bluetooth::Reader reader;


class ScanCallback: public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice) override
  {
      Serial.println(advertisedDevice.getName().c_str());
  }
};

ScanCallback callback;

void setup() 
{
  Serial.begin(115200);
} // End of setup.

void loop() 
{
  Serial.println("Starting scan");
  reader.Scan();
  Serial.println("Scan finished");
} 
