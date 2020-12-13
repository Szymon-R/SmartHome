#include "Utils.hpp"
    
    
bool Converter::HexStringToFloat(const std::string& input, float& inOut)
{
    Buffer<float, 4> buffer;

    if (input.size() != sizeof(buffer))
    {
        Serial.println("Utils: Incorrect input size");
        return false;
    }
    else
    {
        for (int i = 0; i < input.size(); ++i)
        {
            buffer.array[i] = input[i];
        }
    }
    inOut = buffer.value;
    return true;
}

unsigned int Converter::FloatToHexInt(const float value)
{
    Buffer<float, 4> buffer;
    buffer.value = value;
    Buffer<unsigned int, 4> buffer2;
    for (int i = 0; i < 4; ++i)
    {
        buffer2.array[i] = buffer.array[i];
    }
    return buffer2.value;
}