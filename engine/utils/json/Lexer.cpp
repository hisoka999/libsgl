#include "Lexer.h"

namespace utils
{
    namespace JSON
    {
        std::vector<std::string_view> Lexer::lex(std::string_view jsonData)
        {
            std::vector<std::string_view> tokens;
            tokens.reserve(jsonData.size() / 3);

            for (size_t pos = 0; pos < jsonData.size(); ++pos)
            {
                long start = pos;
                long end;
                lex_string(jsonData, &start, &end);
                if (end != -1)
                {
                    auto token = jsonData.substr(start + 1, end - start - 1);
                    tokens.push_back(token);
                    pos = end;
                    continue;
                }

                lex_bool(jsonData, &start, &end);
                if (end != -1)
                {
                    auto token = jsonData.substr(start, end - start);
                    tokens.push_back(token);
                    pos = end - 1;
                    continue;
                }
                lex_null(jsonData, &start, &end);
                if (end != -1)
                {
                    auto token = jsonData.substr(start, end - start);
                    tokens.push_back(token);
                    pos = end - 1;
                    continue;
                }
                lex_number(jsonData, &start, &end);
                if (end != -1 && end - (start - 1) > 0)
                {
                    auto token = jsonData.substr(start, end - (start - 1));
                    tokens.push_back(token);
                    pos = end;
                    continue;
                }
                switch (jsonData[pos])
                {
                case ' ':
                case '\t':
                case '\b':
                case '\n':
                case '\r':
                    break;
                case ',':
                case ':':
                case '[':
                case ']':
                case '{':
                case '}':
                    tokens.push_back(jsonData.substr(pos, 1));
                    break;
                }

                //[JSON_COMMA, JSON_COLON, JSON_LEFTBRACKET, JSON_RIGHTBRACKET,
                //               JSON_LEFTBRACE, JSON_RIGHTBRACE]
            }
            return tokens;
        }

        void Lexer::lex_string(std::string_view jsonData, long *start, long *end)
        {
            if (jsonData[*start] != '"')
            {
                *end = -1;
                return;
            }
            for (size_t pos = *start + 1; pos < jsonData.size() - 1; ++pos)
            {
                if (jsonData[pos] == '"')
                {
                    *end = pos;
                    return;
                }
            }
        }

        void Lexer::lex_bool(std::string_view jsonData, long *start, long *end)
        {
            auto falseString = jsonData.substr(*start, 5);
            auto trueString = jsonData.substr(*start, 4);

            *end = -1;
            if (falseString == "false")
            {
                *end = *start + 5;
            }
            else if (trueString == "true")
            {
                *end = *start + 4;
            }
        }

        void Lexer::lex_null(std::string_view jsonData, long *start, long *end)
        {
            auto nullString = jsonData.substr(*start, 4);
            if (nullString == "null")
            {
                *end = *start + 4;
            }
        }

        void Lexer::lex_number(std::string_view jsonData, long *start, long *end)
        {
            // - . 0..9
            char startChar = jsonData[*start];

            *end = -1;
            if ((startChar >= '0' && startChar <= '9') || startChar == '-')
            {
                for (size_t pos = *start; pos < jsonData.size(); ++pos)
                {
                    startChar = jsonData[pos];
                    if (!((startChar >= '0' && startChar <= '9') || startChar == '.' || startChar == '-'))
                    {
                        *end = pos - 1;
                        return;
                    }
                }
            }
        }

        Lexer::Lexer()
        {
        }
    }
}