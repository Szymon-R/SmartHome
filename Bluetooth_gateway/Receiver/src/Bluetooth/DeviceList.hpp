#ifndef _DEVICE_LIST_HPP
#define _DEVICE_LIST_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include "Device.hpp"

NAMESPACE_START(Bluetooth);

class Devices
{

    public:
        static std::vector<Device> devices;
};

NAMESPACE_END

#endif
