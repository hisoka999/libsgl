#include "object.h"
#include <engine/utils/string.h>
namespace utils
{
    namespace JSON
    {

        std::string convertJsonArrayToString(std::vector<std::variant<int, float, std::string, std::shared_ptr<Object>>> array)
        {
            std::string val = "[\n";
            for (size_t i = 0; i < array.size(); ++i)
            {
                auto data = array[i];
                val += convertJsonDataToString(data);
                if (i + 1 < array.size())
                {
                    val += ",";
                }
                val += "\n";
            }

            val += "]\n";
            return val;
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
            int i = 0;
            for (auto attr : attributes)
            {
                val += "\"" + attr.first + "\":" + convertJsonDataToString(attr.second);
                i++;

                val += ",";
            }
            i = 0;
            for (auto attr : arrayAttributes)
            {
                val += "\"" + attr.first + "\":" + convertJsonArrayToString(attr.second);
                i++;
                val += ",";
            }
            val = utils::rtrim(val, ",");
            val += "}";
            return val;
        }

    }
}
