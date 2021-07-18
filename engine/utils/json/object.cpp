#include "object.h"
#include <engine/utils/string.h>
namespace utils
{
    namespace JSON
    {

        std::string convertJsonDataToString(JsonValue data)
        {
            if (std::holds_alternative<int>(data))
            {
                return std::to_string(std::get<int>(data));
            }
            else if (std::holds_alternative<float>(data))
            {
                return std::to_string(std::get<float>(data));
            }
            else if (std::holds_alternative<bool>(data))
            {
                bool value = std::get<bool>(data);
                return (value) ? "true" : "false";
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

        std::string convertJsonArrayToString(JsonArray array)
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

        float Object::getFloatValue(const std::string &attr) const
        {
            return std::get<float>(attributes.at(attr));
        }

        bool Object::getBoolValue(const std::string &attr) const
        {
            return std::get<bool>(attributes.at(attr));
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

        bool Object::hasAttribute(const std::string &attr)
        {
            return attributes.count(attr) > 0;
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
