#ifndef PARSER_H
#define PARSER_H

#include "../string.h"
#include "object.h"
#include <memory>
#include <string>
#include <vector>
#include <list>
namespace utils
{

    namespace JSON
    {

        class Parser
        {
        public:
            Parser();

            void parseString(const std::string &jsonData);
            JsonArray parseArray(const std::string_view &jsonData);
            //std::shared_ptr<Object> parseObject(const std::string &jsonData);

            std::shared_ptr<Object> parseObject(const std::string &jsonData);

        private:
            void parseAttribute(const std::string &jsonData);
            size_t findPositionInString(const std::string_view &data, char starChar, char endChar, const size_t startPosition);

            const std::shared_ptr<Object> parse_object(std::vector<std::string_view> &tokens, size_t *start);
            JsonArray parse_array(std::vector<std::string_view> &tokens, size_t *start);
            JsonValue parse_value(const std::string_view &token);
        };
    }
}

#endif // PARSER_H
