#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <string>
#include <Arduino.h>


#define NAMESPACE_START(name) \
    namespace name \
    {

#define NAMESPACE_END \
    }
template<typename T, uint8_t bufferSize>
union Buffer
{
    char array[bufferSize];
    T value;
};



class Converter
{
    public:
        bool HexStringToFloat(const std::string& input, float& inOut);
        unsigned int FloatToHexInt(const float value);
};

#endif