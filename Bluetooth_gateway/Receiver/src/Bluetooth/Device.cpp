#include "Device.hpp"



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