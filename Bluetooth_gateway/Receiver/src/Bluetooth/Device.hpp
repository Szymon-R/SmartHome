#include "Utils.hpp"
#include "Logger.hpp"
#include <vector>

#ifndef _DEVICE_HPP
#define _DEVICE_HPP

NAMESPACE_START(Bluetooth)

struct Value
{
    std::string timeStamp;
    std::string value;
};


class Characteristic
{
    public:
        Characteristic(const std::string& charName, const std::string& charCode) :
            charName(charName), charCode(charCode)
        {

        }  
        const std::string charName;
        const std::string charCode;
        
        void InsertValue(const std::string& value)
        {
            Value val;
            val.value = value;
            this->values.emplace_back(val);
        }

    private:
        std::vector<Value> values;
};


class Service
{
    public:
        Service(const std::string& serviceName, const std::string& serviceCode, const std::initializer_list<Characteristic>& characteritics) :
            serviceName(serviceName), serviceCode(serviceCode), characteritics(characteritics)
        {
        }
        const std::string serviceName;
        const std::string serviceCode;

        Characteristic* GetCharacteristic(const std::string& name)
        {
            Characteristic* out = nullptr;
            for(auto cha : this->characteritics)
            {
                if (cha.charName == name)
                {
                    out = const_cast<Characteristic*>(&cha);
                    break;
                }
            }
            if (!out)
            {
                LOG_MEDIUM("Didn't find characteristic: ", name, " in service: ", this->serviceName, "\n\r");
            }
            return out;
        }
        Characteristic* operator[](const std::string& name)
        {
            Characteristic* out = nullptr;
            for(auto cha : this->characteritics)
            {
                if (cha.charName == name)
                {
                    out = const_cast<Characteristic*>(&cha);
                    break;
                }
            }
            if (!out)
            {
                LOG_MEDIUM("Didn't find characteristic: ", name, " in service: ", this->serviceName, "\n\r");
            }
            return out;
        }
    private:
        const std::vector<Characteristic> characteritics;
};


class Device
{
    public:
        Device(const std::string& deviceName, const std::initializer_list<Service>& services) :
            deviceName(deviceName), services(services)
        {
        }
    const std::string deviceName;
    Service* GetService(const std::string& name)
    {
        Service* out = nullptr;
        for(auto& service : this->services)
        {
            if (service.serviceName == name)
            {
                out = const_cast<Service*>(&service);
                break;
            }
        }
        if (!out)
        {
            LOG_MEDIUM("Didn't find service: ", name, " in device: ", this->deviceName, "\n\r");
        }
        return out;
    }
    Service* operator[](const std::string& name)
    {
        Service* out = nullptr;
        for(auto& service : this->services)
        {
            if (service.serviceName == name)
            {
                out = const_cast<Service*>(&service);
                break;
            }
        }
        if (!out)
        {
            LOG_MEDIUM("Didn't find service: ", name, " in device: ", this->deviceName, "\n\r");
        }
        return out;
    }
    void InsertValue(Service* service, Characteristic* characteristic, const std::string& value)
    {
        this->GetService(service->serviceName)->GetCharacteristic(characteristic->charName)->InsertValue(value);
    }
    private:
        const std::vector<Service> services;
};

NAMESPACE_END

#endif