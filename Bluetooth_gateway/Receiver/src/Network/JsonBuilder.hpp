#ifndef _JSON_BUILDER_HPP
#define _JSON_BUILDER_HPP

#include "Json.hpp"
#include <string>
#include <vector>
#include "../Utils/Utils.hpp"
#include "../Bluetooth/Device.hpp"

NAMESPACE_START(Network)

class JsonBuilder
{
    public:
        static Json Create(const Bluetooth::Device& dev);
        static Json Create(const Bluetooth::Service& serv);
        static Json Create(const Bluetooth::Characteristic& chr);
        static Json Create(const Bluetooth::Value& val);
        static std::string Parse(const Json& j);

    private:
        static std::string Indent(std::string element);
        static std::string Parse(const std::vector<Element>& elements);
};

NAMESPACE_END

#endif