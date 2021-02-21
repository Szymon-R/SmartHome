#ifndef _JSON_HPP
#define _JSON_HPP

#include "Json.hpp"

using namespace Network;

Element::Element(int val)
{
    this->type = DATA_TYPE::INT;
    this->intVal = val;
}

Element::Element(const std::string& val)
{
    this->type = DATA_TYPE::STRING;
    this->stringVal = val;
}

Element::operator std::string() const
{
    if (this->type == DATA_TYPE::INT)
    {
        char buffer[12];
        std::sprintf(buffer, "%d", this->intVal);
        std::string str = buffer;
        return str;
    }
    else if (this->type == DATA_TYPE::STRING)
    {
        return "\"" + this->stringVal + "\"";
    }
}

Json::Json(std::string tag, Json json)
{
    this->tag = tag;
    this->jsons.emplace_back(json);
}

Json::Json(std::string tag, Element el)
{
    this->tag = tag;
    this->elements.emplace_back(el);
}

Json::Json(std::string tag, const std::initializer_list<Element>& el)
{
    this->tag = tag;
    this->elements = el;
}

Json::Json(std::string tag, std::vector<Element>& el)
{
    this->tag = tag;
    this->elements = std::move(el);
}

Json::Json(std::string tag, const std::initializer_list<Json>& el)
{
    this->tag = tag;
    this->jsons = el;
}

Json::Json(std::string tag, std::vector<Json> el)
{
    this->tag = tag;
    this->jsons = std::move(el);
}




#endif