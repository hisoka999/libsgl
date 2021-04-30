#ifndef JSONSERVICE_H
#define JSONSERVICE_H

#include <engine/utils/exceptions.h>
#include <engine/utils/json/parser.h>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

namespace utils
{
    namespace json
    {

        template <class T>
        class JSONService
        {
        public:
            JSONService() {}
            void loadData(const std::string &fileName)
            {
                std::ifstream file;
                std::istringstream is;
                std::string s;
                std::string group;
                //  std::cout << filename << std::endl;

                file.open(fileName.c_str(), std::ios::in);
                if (!file.is_open())
                {
                    throw IOException(fileName, "file does not exists");
                }
                file.seekg(0, std::ios::end);
                size_t size = file.tellg();
                std::string buffer(size, ' ');
                file.seekg(0);
                file.read(&buffer[0], size);

                //std::cout << buffer << std::endl;

                auto objects = parser.parseArray(buffer);

                for (auto &object : objects)
                {
                    data.push_back(convertJsonObject2Data(std::get<std::shared_ptr<utils::JSON::Object>>(object)));
                }
            }

            const std::vector<std::shared_ptr<T>> getData() const
            {
                return data;
            }

        protected:
            virtual std::shared_ptr<T> convertJsonObject2Data(const std::shared_ptr<utils::JSON::Object> &object)
            {
                return nullptr;
            }

            void setData(const std::vector<std::shared_ptr<T>> &data)
            {
                this->data = data;
            }

        private:
            utils::JSON::Parser parser;
            std::vector<std::shared_ptr<T>> data;
        };
    }
}

#endif // JSONSERVICE_H
