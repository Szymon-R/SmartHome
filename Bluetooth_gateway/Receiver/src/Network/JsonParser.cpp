#include "JsonParser.hpp"

using namespace Network;

std::string JsonParser::Parse(const Json& j)
{
    std::stringstream stream;
    static int spacebarCounter = 0;

    stream << JsonParser::Indent(spacebarCounter);
    
    stream << JsonParser::Apostrof(j.tag) << ": ";

    if (j.jsons.size() > 0)
    {
        stream << "{";
        ++spacebarCounter;
    }

    for (int i = 0; i < j.jsons.size(); ++i)
    {
        stream << "\n\r";
        stream << JsonParser::Parse(j.jsons[i]);
        if (j.jsons.size() > 1 && i < j.jsons.size() - 1 )
        {
            stream << ",";
        }
    }

    if (j.jsons.size() > 0)
    {
        stream << "\n\r";
        stream << JsonParser::Indent(--spacebarCounter);
        stream << "}";
    }
    stream << JsonParser::Parse(j.elements);

    return stream.str() ;
}


std::string JsonParser::Indent(const int counter)
{
    std::string str;
    str.reserve(counter);
    for (int i=0; i < counter; ++i)
    {
        str += "  ";
    }
    return str;
}

std::string JsonParser::Apostrof(std::string element)
{
    return "\"" + element + "\"";
}


std::string JsonParser::Parse(const std::vector<Element>& elements)
{
    std::stringstream stream;
    if (elements.size() > 0)
    {
        if (elements.size() > 1)
        {
            stream << "[";
        }
        stream << elements[0].operator std::string();
        for (unsigned int i = 1; i < elements.size(); ++i)
        {
            stream << ", " << elements[i].operator std::string();
        }
        if (elements.size() > 1)
        {
            stream << "]";
        }
    }
    return stream.str();
}