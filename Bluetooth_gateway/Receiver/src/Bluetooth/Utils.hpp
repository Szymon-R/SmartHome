#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <string>
#include <Arduino.h>
#include <type_traits>
#include <sstream>


#define LOG_LEVEL_LOW

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

namespace std
{
    template< bool B, class T = void >
    using enable_if_t = typename std::enable_if<B,T>::type;

    template< class T >
    constexpr bool is_integral_v = std::is_integral<T>::value;

    template< class T >
    using remove_reference_t = typename std::remove_reference<T>::type;

    template< class T, class U >
    constexpr bool is_same_v = std::is_same<T, U>::value;

    template< class T >
    using remove_cv_t = typename std::remove_cv<T>::type;

    template< class T >
    constexpr bool is_floating_point_v = std::is_floating_point<T>::value;
}


class Converter
{
    public:
        bool HexStringToFloat(const std::string& input, float& inOut);
        unsigned int FloatToHexInt(const float value);
};

#endif