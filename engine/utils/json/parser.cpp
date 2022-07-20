#include "parser.h"
#include <algorithm>
#include "Lexer.h"
#include <chrono>
#include "engine/utils/logger.h"
namespace utils
{
    namespace JSON
    {

        Parser::Parser()
        {
        }

        void Parser::parseString(const std::string &jsonData)
        {

            for (size_t i = 0; i < jsonData.length(); ++i)
            {
                char data = jsonData[i];
                switch (data)
                {
                case '[': // array begin
                    // parse sub string
                    parseArray(jsonData.substr(i + 1));
                    break;
                case ']': // array end
                    break;
                case '{': // object begin
                    parseObject(jsonData.substr(i + 1));
                    break;
                case '}': // object end
                    break;
                case ':': // object end
                    break;
                }
            }
        }

        size_t Parser::findPositionInString(const std::string_view &data, char starChar, char endChar, const size_t startPosition)
        {
            int depth = 0;
            for (size_t pos = startPosition; pos < data.size(); ++pos)
            {
                if (data[pos] == starChar)
                {
                    depth++;
                }
                else if (data[pos] == endChar)
                {
                    if (depth > 0)
                    {
                        depth--;
                    }
                    else
                    {
                        return pos;
                    }
                }
            }
            return 0;
        }

        JsonArray Parser::parseArray(const std::string_view &jsonData)
        {
            JsonArray vector;
            utils::JSON::Lexer lexer;

            auto tokens = lexer.lex(jsonData);

            // t = tokens[0]

            // if(isRoot && tokens[0] == "{")
            size_t start = 0;
            if (tokens[start] == "[")
            {
                start++;
                return parse_array(tokens, &start);
            }
            return vector;
        }

        JsonValue Parser::parse_value(const std::string_view &token)
        {
            auto data = token.data();
            char startChar = token[0];

            if (token == "true")
            {
                return JsonValue(true);
            }
            else if (token == "false")
            {
                return JsonValue(false);
            }
            else if (token == "null")
            {
                return JsonValue(0);
            }
            else if ((startChar >= '0' && startChar <= '9') || startChar == '-')
            {
                if (token.find_first_of(".") < token.size())
                {
                    return float(std::atof(data));
                }
                else
                {
                    auto value = std::atoi(data);
                    if (token.size() > 64)
                    {
                        return std::string(token);
                    }
                    return value;
                }
            }
            else
            {
                return std::string(token);
            }
        }

        JsonArray Parser::parse_array(std::vector<std::string_view> &tokens, size_t *start)
        {
            JsonArray array;
            auto token = tokens[*start];

            if (token == "]")
            {
                *start = *start + 1;
                return array;
            }

            while (true)
            {
                token = tokens[*start];
                if (token == "{")
                {
                    *start = *start + 1;
                    array.push_back(parse_object(tokens, start));
                }
                else
                {
                    JsonValue value = parse_value(token);
                    array.push_back(value);
                    *start = *start + 1;
                }
                token = tokens[*start];
                if (token == ",")
                    *start = *start + 1;
                token = tokens[*start];
                if (token == "]")
                {
                    *start = *start + 1;
                    return array;
                }
            }
            return array;
        }

        const std::shared_ptr<Object> Parser::parse_object(std::vector<std::string_view> &tokens, size_t *start)
        {
            auto token = tokens[*start];

            std::shared_ptr<Object> jsonObject = std::make_shared<Object>();
            if (token == "}")
            {
                *start = *start + 1;
                return jsonObject;
            }

            while (true)
            {

                auto jsonKey = std::string(tokens[*start]);

                *start = *start + 1; // erease key

                if (tokens[*start] != ":")
                {
                    throw std::runtime_error(std::string("expected  colon but got other token: ") + std::string(tokens[*start]) + "for key: " + jsonKey);
                }

                *start = *start + 1; // erease colon

                auto valueStart = tokens[*start];

                if (valueStart == "{")
                {
                    *start = *start + 1;
                    jsonObject->setAttribute(jsonKey, parse_object(tokens, start));
                }
                else if (valueStart == "[")
                {
                    *start = *start + 1;
                    auto arrayValue = parse_array(tokens, start);
                    jsonObject->setArrayAttribute(jsonKey, arrayValue);
                }
                else
                {
                    JsonValue value = parse_value(valueStart);
                    jsonObject->setAttribute(jsonKey, value);
                    *start = *start + 1;
                }

                auto token = tokens[*start];

                if (token == "}")
                {
                    *start = *start + 1;
                    return jsonObject;
                }
                else if (token != ",")
                {
                    throw std::runtime_error(std::string("expected  comma but got other token: ") + std::string(tokens[*start]) + " for key: " + jsonKey + " for Value(" + std::string(valueStart) + ")");
                }

                *start = *start + 1;
            }
            return nullptr;
        }

        std::shared_ptr<Object> Parser::parseObject(const std::string &jsonData)
        {

            auto start = std::chrono::high_resolution_clock::now();

            utils::JSON::Lexer lexer;

            auto tokens = lexer.lex(jsonData);

            auto elapsed = std::chrono::high_resolution_clock::now() - start;

            long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            SGL_LOG_TRACE("after lex: " + std::to_string(milliseconds) + "ms");
            start = std::chrono::high_resolution_clock::now();

            size_t startPos = 0;

            if (tokens[startPos] == "{")
            {
                startPos++;
                auto result = parse_object(tokens, &startPos);
                elapsed = std::chrono::high_resolution_clock::now() - start;

                milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
                SGL_LOG_TRACE("after parse_object: " + std::to_string(milliseconds) + "ms");
                return result;
            }

            return nullptr;
        }

    }

}
