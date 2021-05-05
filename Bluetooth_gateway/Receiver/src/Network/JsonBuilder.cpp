#include "JsonBuilder.hpp"


using namespace Network;


Json JsonBuilder::Create(const std::vector<Bluetooth::Device>& devices)
{
    std::vector<Json> jsons;
    jsons.reserve(devices.size());
    for(size_t i = 0; i < devices.size(); ++i)
    {
        jsons[i] = JsonBuilder::Create(devices[i]);
    }
    return Json{"Devices", jsons};
}



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
    while (chr.ReadValue(val, Bluetooth::Mode::READ))
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


std::string JsonBuilder::GetTag(std::string& data)
{
    // std::regex reg("^\".\")
    return "";
}