#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "Json.hpp"
#include <string>
#include <vector>
#include "../Utils/Utils.hpp"
#include "../Bluetooth/Device.hpp"
#include <regex>

NAMESPACE_START(Network)

class JsonParser
{
    public:
        static std::string Parse(const Json& j);
    
    private:
        static std::string Indent(const int counter);
        static std::string Apostrof(std::string element);
        static std::string Parse(const std::vector<Element>& elements);
};

NAMESPACE_END

#endif