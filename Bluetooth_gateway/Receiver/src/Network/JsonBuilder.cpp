#include "JsonBuilder.hpp"


using namespace Network;


Json JsonBuilder::Create(const Bluetooth::Device& dev)
{
    auto services = dev.GetServices();

    std::vector<Json> jServices;
    jServices.reserve(dev.ServCount() + 2);

    jServices.push_back(Json{"device_name:", dev.deviceName});
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


Json JsonBuilder::Create(const Bluetooth::Characteristic& chr)
{
    Bluetooth::Value val;
    std::vector<Json> values;
    values.reserve(chr.ValuesCount() + 2);
    while (chr.GetValue(val))
    {
        values.push_back(JsonBuilder::Create(val));
    }
    values.push_back(Json{"char_name", chr.charName});
    values.push_back(Json{"char_code", chr.charCode});
    return Json{"Characteristic", values};
}

Json JsonBuilder::Create(const Bluetooth::Value& val)
{
    Json j1{"time_stamp", val.timeStamp};
    Json j2{"value", val.value};
    return Json{"Value", {j1, j2}};
}

std::string JsonBuilder::Parse(const Json& j)
{
    std::stringstream stream;
    
    stream << "{" << JsonBuilder::Indent(j.tag) << ": ";
    for (auto& element : j.jsons)
    {
        stream << JsonBuilder::Parse(element);
    }
    stream << JsonBuilder::Parse(j.elements);

    return stream.str();
}

std::string JsonBuilder::Indent(std::string element)
{
    return "\"" + element + "\"";
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
        stream << "\n  ";
    }
    return stream.str();
}