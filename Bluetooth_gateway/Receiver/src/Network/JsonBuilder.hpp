#ifndef _JSON_BUILDER_HPP
#define _JSON_BUILDER_HPP

#include "Json.hpp"
#include <string>
#include <vector>
#include "../Utils/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include <regex>

NAMESPACE_START(Network)

class JsonBuilder
{
    public:
        static Json Create(const std::vector<Bluetooth::Device>& devices);
        static Json Create(const Bluetooth::Device& dev);
        static Json Create(const Bluetooth::Service& serv);
        static Json Create(Bluetooth::Characteristic& chr);
        static Json Create(const Bluetooth::Value& val);
        static Json Create(const std::string& jsonData);

    private:
        static std::pair<std::string, std::string> Split(std::string& data);
        static std::string GetTag(std::string& data);
};

NAMESPACE_END

#endif