
#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include <list>
#include <vector>

NAMESPACE_START(Bluetooth)

enum class CharStatus
{
    TO_BE_READ,
    TO_BE_WRITTEN,
    CHAR_PROPERTY_MISMATCH,
    SUCCESS,
    TIMEOUT
};

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
            this->InsertToBeRead("1000");
            this->InsertToBeRead("2000");
        }

        const std::string charName;
        const std::string charCode;

        void InsertToBeRead(const std::string& value)
        {
            Value val;
            val.value = value;
            //LOG_LOW("Inserting value: ", val.value, "\n\r");
            this->readBuffer.push_back(val);
        }

        void InsertToBeWritten(const std::string& value)
        {
            Value val;
            val.value = value;
            //LOG_LOW("Inserting value: ", val.value, "\n\r");
            this->writeBuffer.push_back(val);
        }

        bool GetValue(Value& buffer)
        {
            Value val;
            if (readFromDeviceValues.size() > 0)
            {
                buffer = readFromDeviceValues.front();
                readFromDeviceValues.pop_front();
                return true;
            }
            return false;          
        }

        uint32_t ReadValuesCount() const
        {
            return this->readFromDeviceValues.size();
        }

        void SetStatus(CharStatus status)
        {
            this->status = status;
        }

        CharStatus GetStatus(bool reset = false)
        {
            CharStatus temp = this->status;
            if (reset)
            {
                this->status = CharStatus::TO_BE_READ;
            }
            return temp;
        }

    private:
        CharStatus status = CharStatus::TO_BE_READ;
        std::list<Value> readBuffer;
        std::list<Value> writeBuffer;
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
            for(auto& cha : this->characteritics)
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
        const std::list<Characteristic>& GetCharacteristics() const
        {
            return this->characteritics;
        }
        Characteristic* operator[](const std::string& name)
        {
            Characteristic* out = nullptr;
            for(auto& cha : this->characteritics)
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
        uint32_t CharCount() const
        {
            return this->characteritics.size();
        }

    private:
        std::list<Characteristic> characteritics;
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
    const std::list<Service>& GetServices() const
    {
        return this->services;
    }

    std::vector<std::pair<Service*, std::vector<Characteristic*>>> GetAll()
    {
        std::vector<std::pair<Service*, std::vector<Characteristic*>>> out;
        for(auto& serv : services)
        {
            auto chars = serv.GetCharacteristics();
            std::vector<Characteristic*> charas;
            for (auto& chara : chars)
            {
                charas.emplace_back(const_cast<Characteristic*>(&chara));
            }
            out.push_back(make_pair(const_cast<Service*>(&serv), charas));
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
    void InsertValue(Characteristic* characteristic, const std::string& value)
    {
        characteristic->InsertValue(value);
    }
    uint32_t ServCount() const
    {
        return services.size();
    }
    private:
        std::list<Service> services;
};

NAMESPACE_END

#endif