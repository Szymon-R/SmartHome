#include "DeviceList.hpp"

// Device info might be stored in xml, json or any other format

using namespace Bluetooth;

std::vector<Device> Devices::devices
(
    {
        Device
        {
            "Temperature sensor",
            {
            Service
                {"Service1",
                "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
                    {
                    Characteristic
                        {"Characteristic1",
                        "beb5483e-36e1-4688-b7f5-ea07361b26a8"
                        }
                    }
                }
            }
        },

        Device
        {
            "Water sensor",
            {
            Service
                {"Service1",
                "ffa8fabe-24be-40b4-b234-01c08ce2b3eb",
                    {
                    Characteristic
                        {"Characteristic1",
                        "09836e84-a18c-4692-a361-8e7e535f8874"
                        },
                    Characteristic
                        {"Characteristic2",
                        "472f697e-b7f5-41d5-908e-bd2a74020b29"
                        }
                    }
                },
                Service
                {"Service1",
                "f7c09b94-64d6-4d90-8d7c-83b92c5c0f30",
                    {
                    Characteristic
                        {"Characteristic1",
                        "83e577b7-931a-44d0-81eb-1b1910bfd349"
                        }
                    }
                }
            }
        },
    }
) ;

