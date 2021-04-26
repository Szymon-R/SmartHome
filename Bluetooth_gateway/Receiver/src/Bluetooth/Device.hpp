
#ifndef _DEVICE_HPP
#define _DEVICE_HPP

#include "../Utils/Utils.hpp"
#include "../Utils/Logger.hpp"
#include <list>
#include <vector>

NAMESPACE_START(Bluetooth)

enum class CharStatus
{
    READY,
    TIMEOUT,
    SUCCESS,
    INVALID
};

struct Mode
{
    static constexpr uint8_t READ = 0x01;
    static constexpr uint8_t WRITE = 0x02;
    static constexpr uint8_t NOTIFY = 0x04;
};

struct Value
{
    std::string timeStamp;
    std::string value;
};

class Characteristic
{
    public:
        Characteristic(const std::string& charName, const std::string& charCode);
        void WriteValue(const std::string& value, uint8_t mode);
        bool ReadValue(Value& buffer, uint8_t mode);
        uint32_t ReadValuesCount() const;
        void SetStatus(CharStatus status);
        CharStatus GetStatus();
        void SetMode(uint8_t mode);
        uint8_t GetMode();
        const std::string charName;
        const std::string charCode;

    private:
        uint8_t mode;
        CharStatus status = CharStatus::READY;
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