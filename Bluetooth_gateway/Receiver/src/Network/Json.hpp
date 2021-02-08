#ifndef _JSON2_HPP
#define _JSON2_HPP

#include <string>
#include <vector>
#include "../Utils/Utils.hpp"

NAMESPACE_START(Network)

struct Element
{
    enum class DATA_TYPE
    {
        INT,
        STRING,
        DOUBLE
    };

    Element(int val);
    Element(const std::string& val);
    DATA_TYPE type;
    operator std::string() const;
    std::string stringVal;
    int intVal;
};



struct Json
{
    Json(std::string tag, Element el);
    Json(std::string tag, const std::initializer_list<Element>& el);
    Json(std::string tag, std::vector<Element>& el);

    Json(std::string tag, Json json);
    Json(std::string tag, const std::initializer_list<Json>& el);
    Json(std::string tag, std::vector<Json> el);

    std::string tag;
    std::vector<Element> elements;
    std::vector<Json> jsons;

};

NAMESPACE_END

#endif