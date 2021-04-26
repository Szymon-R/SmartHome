#include "Device.hpp"

using namespace Bluetooth;

Characteristic::Characteristic(const std::string& charName, const std::string& charCode) :
    charName(charName), charCode(charCode)
{
    this->WriteValue("1000", Mode::READ);
    this->WriteValue("2000", Mode::READ);
}

void Characteristic::WriteValue(const std::string& value, uint8_t mode)
{
    if (mode == Mode::READ)
    {
        Value val;
        val.value = value;
        //LOG_LOW("Inserting value: ", val.value, "\n\r");
        this->readBuffer.push_back(val);
    }

    else if (mode == Mode::WRITE)
    {
        Value val;
        val.value = value;
        //LOG_LOW("Inserting value: ", val.value, "\n\r");
        this->writeBuffer.push_back(val);
    }
}

bool Characteristic::ReadValue(Value& buffer, uint8_t mode)
{
    if (mode == Mode::READ)
    {
        if (this->readBuffer.size() > 0)
        {
            buffer = this->readBuffer.front();
            this->readBuffer.pop_front();
            return true;
        }
        return false;    
    }
    else if (mode == Mode::WRITE)
    {
        if (this->writeBuffer.size() > 0)
        {
            buffer = writeBuffer.front();
            this->writeBuffer.pop_front();
            return true;
        }
        return false;    
    }
}

uint32_t Characteristic::ReadValuesCount() const
{
    return this->readBuffer.size();
}

void Characteristic::SetStatus(CharStatus status)
{
    this->status = status;
}

CharStatus Characteristic::GetStatus()
{
    return this->status;
}

void Characteristic::SetMode(uint8_t mode)
{
    this->mode = mode;
}

uint8_t Characteristic::GetMode()
{
    return this->mode;
}