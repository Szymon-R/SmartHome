#include "JsonBuilder.hpp"


using namespace Network;


Json JsonBuilder::Create(const Bluetooth::Device& dev)
{
    auto services = dev.GetServices();

    std::vector<Json> jServices;
    jServices.reserve(dev.ServCount() + 1);

    jServices.push_back(Json{"device_name", dev.deviceName});
    for (auto& srv : services)
    {
        jServices.push_back(JsonBuilder::Create(srv));
    }

    return Json{"Device", jServices};
}


Json JsonBuilder::Create(const Bluetooth::Service& serv)
{
    std::vector<Json> jchar;
    jchar.reserve(serv.CharCount() + 2);
    auto characteristics = serv.GetCharacteristics();
    jchar.push_back(Json{"serv_name", serv.serviceName});
    jchar.push_back(Json{"serv_code", serv.serviceCode});
    for (auto& chr : characteristics)
    {
        jchar.push_back(JsonBuilder::Create(chr));
    }

    return Json{"Service", jchar};
}


Json JsonBuilder::Create(Bluetooth::Characteristic& chr)
{
    Bluetooth::Value val;
    std::vector<Json> values;
    values.reserve(chr.ReadValuesCount() + 2);
    values.push_back(Json{"char_name", chr.charName});
    values.push_back(Json{"char_code", chr.charCode});
    while (chr.GetValue(val))
    {
        values.push_back(JsonBuilder::Create(val));
    }

    return Json{"Characteristic", values};
}

Json JsonBuilder::Create(const Bluetooth::Value& val)
{
    Json j1{"time_stamp", val.timeStamp};
    Json j2{"value", val.value};
    return Json{"Value", {j1, j2}};
}

Json JsonBuilder::Create(const std::string& jsonData)
{
    std::stringstream stream;
    static int spacebarCounter = 0;
    
    

}

std::string JsonBuilder::Parse(const Json& j)
{
    std::stringstream stream;
    static int spacebarCounter = 0;

    stream << JsonBuilder::Indent(spacebarCounter);
    
    stream << JsonBuilder::Apostrof(j.tag) << ": ";

    if (j.jsons.size() > 0)
    {
        stream << "{";
        ++spacebarCounter;
    }

    for (int i = 0; i < j.jsons.size(); ++i)
    {
        stream << "\n\r";
        stream << JsonBuilder::Parse(j.jsons[i]);
        if (j.jsons.size() > 1 && i < j.jsons.size() - 1 )
        {
            stream << ",";
        }
    }

    if (j.jsons.size() > 0)
    {
        stream << "\n\r";
        stream << JsonBuilder::Indent(--spacebarCounter);
        stream << "}";
    }
    stream << JsonBuilder::Parse(j.elements);

    return stream.str() ;
}

std::string JsonBuilder::Apostrof(std::string element)
{
    return "\"" + element + "\"";
}

std::string JsonBuilder::Indent(const int counter)
{
    std::string str;
    str.reserve(counter);
    for (int i=0; i < counter; ++i)
    {
        str += "  ";
    }
    return str;
}
std::string JsonBuilder::Parse(const std::vector<Element>& elements)
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

std::string JsonBuilder::GetTag(std::string& data)
{
    // std::regex reg("^\".\")
    return "";
}