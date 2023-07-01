#include "object.h"
#include <engine/utils/string.h>
#include "engine/utils/logger.h"
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
            else if (std::holds_alternative<double>(data))
            {
                return std::to_string(std::get<double>(data));
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
                if (obj == nullptr)
                    return "null";
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
            if (std::holds_alternative<int>(attributes.at(attr)))
            {
                return static_cast<float>(std::get<int>(attributes.at(attr)));
            }
            return static_cast<float>(std::get<double>(attributes.at(attr)));
        }

        double Object::getDoubleValue(const std::string &attr) const
        {
            if (std::holds_alternative<int>(attributes.at(attr)))
            {
                return static_cast<double>(std::get<int>(attributes.at(attr)));
            }
            return std::get<double>(attributes.at(attr));
        }

        bool Object::getBoolValue(const std::string &attr) const
        {
            return std::get<bool>(attributes.at(attr));
        }
        std::shared_ptr<Object> Object::getObjectValue(const std::string &attr) const
        {
            try
            {
                return std::get<std::shared_ptr<Object>>(attributes.at(attr));
            }
            catch (std::bad_variant_access const &ex)
            {
                SGL_LOG_WARN(std::string(ex.what()) + ": " + attr + " is empty");
                return nullptr;
            }
        }

        JsonArray Object::getArray(const std::string &attr) const
        {
            return arrayAttributes.at(attr);
        }

        bool Object::hasArray(const std::string &attr)
        {
            return arrayAttributes.count(attr) > 0;
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
