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
        static Json Create(const Bluetooth::Device& dev);
        static Json Create(const Bluetooth::Service& serv);
        static Json Create(Bluetooth::Characteristic& chr);
        static Json Create(const Bluetooth::Value& val);
        static Json Create(const std::string& jsonData);
        static std::string Parse(const Json& j);

    private:
        static std::string Apostrof(std::string element);
        static std::string Parse(const std::vector<Element>& elements);
        static std::string Indent(const int couter);
        
        static std::pair<std::string, std::string> Split(std::string& data);
        static std::string GetTag(std::string& data);
};

NAMESPACE_END

#endif