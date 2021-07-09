#include "object.h"
namespace utils
{
    namespace JSON
    {

        std::string convertJsonArrayToString(std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> array)
        {
            std::string val = "[\n";
            for (auto data : array)
            {
                val += convertJsonDataToString(data);
                val += "\n,\n";
            }

            val += "]\n";
            return "";
        }
        std::string convertJsonDataToString(std::variant<int, float, std::string, std::shared_ptr<Object>> data)
        {
            if (std::holds_alternative<int>(data))
            {
                return std::to_string(std::get<int>(data));
            }
            else if (std::holds_alternative<float>(data))
            {
                return std::to_string(std::get<float>(data));
            }
            else if (std::holds_alternative<std::string>(data))
            {
                return "\"" + std::get<std::string>(data) + "\"";
            }
            else
            {
                std::shared_ptr<Object> obj = std::get<std::shared_ptr<Object>>(data);
                return obj->toJsonString();
            }
            return "";
        }

        Object::Object()
        {
        }

        std::string Object::getStringValue(const std::string &attr) const
        {
            return std::get<std::string>(attributes.at(attr));
        }
        int Object::getIntValue(const std::string &attr) const
        {
            return std::get<int>(attributes.at(attr));
        }
        std::shared_ptr<Object> Object::getObjectValue(const std::string &attr) const
        {
            return std::get<std::shared_ptr<Object>>(attributes.at(attr));
        }

        JsonArray Object::getArray(const std::string &attr) const
        {
            return arrayAttributes.at(attr);
        }
        void Object::setArrayAttribute(const std::string &attr, JsonArray array)
        {
            arrayAttributes[attr] = array;
        }

        std::vector<std::string> Object::getAttributes()
        {
            std::vector<std::string> result;

            for (auto attr : attributes)
            {
                result.push_back(attr.first);
            }
            return result;
        }

        std::string Object::toJsonString()
        {
            std::string val = "{\n";
            for (auto attr : attributes)
            {
                val += "\"" + attr.first + "\":" + convertJsonDataToString(attr.second) + ",\n";
            }
            for (auto attr : arrayAttributes)
            {
                val += "\"" + attr.first + "\":" + convertJsonArrayToString(attr.second) + ",\n";
            }
            val += "}";
            return val;
        }

    }
}
