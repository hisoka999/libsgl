#pragma once

#include <string>

namespace utils
{
    namespace JSON
    {
        enum class TokenType
        {
            STRING,
            NUMBER,
            BOOL,
            NULLTOKEN,
            OTHER
        };

        struct Token
        {
            TokenType type;
            std::string_view data;
        };

    }
}