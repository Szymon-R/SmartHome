#include "Utils.hpp"
#include <vector>

#ifndef _DEVICE_HPP
#define _DEVICE_HPP

NAMESPACE_START(Bluetooth)

class Value
{
    std::string timeStamp;
    std::string value;
};


class Characteristic
{
    public:
        Characteristic(const std::string& charName) :
            charName(charName)
        {

        }  
        const std::string charName;

    private:
        std::vector<Value> values;
};


class Service
{
    public:
        Service(const std::string& serviceName, const std::initializer_list<Characteristic>& characteritics) :
            serviceName(serviceName), characteritics(characteritics)
        {
        }
        const std::string serviceName;

    private:
        std::vector<Characteristic> characteritics;
};



class Device
{
    public:
        Device(const std::string& deviceName, const std::initializer_list<Service>& services) :
            deviceName(deviceName), services(services)
        {
        }
    const std::string deviceName;

    private:
        
        std::vector<Service> services;
};

NAMESPACE_END

#endif