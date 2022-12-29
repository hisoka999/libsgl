#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace utils
{
    namespace JSON
    {

        class Object;

        typedef std::variant<bool, int, double, std::string, std::shared_ptr<Object>> JsonValue;
        typedef std::vector<JsonValue> JsonArray;

        class Object
        {
        public:
            Object();

            template <typename T>
            void setAttribute(const std::string &attr, T value)
            {
                attributes[attr] = value;
            }

            void setArrayAttribute(const std::string &attr, JsonArray array);

            std::string getStringValue(const std::string &attr) const;
            int getIntValue(const std::string &attr) const;
            float getFloatValue(const std::string &attr) const;
            double getDoubleValue(const std::string &attr) const;
            bool getBoolValue(const std::string &attr) const;
            std::shared_ptr<Object> getObjectValue(const std::string &attr) const;
            JsonArray getArray(const std::string &attr) const;
            bool hasArray(const std::string &attr);

            std::vector<std::string> getAttributes();
            bool hasAttribute(const std::string &attr);

            std::string toJsonString();

        private:
            std::map<std::string, JsonValue> attributes;
            std::map<std::string, JsonArray> arrayAttributes;
        };
    }
}

#endif // OBJECT_H
