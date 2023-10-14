#ifndef __LEXER_H__
#define __LEXER_H__

#include <list>
#include <vector>
#include <string>
#include "Token.h"

namespace utils
{
    namespace JSON
    {

        class Lexer
        {
        public:
            Lexer();

            std::vector<Token> lex(std::string_view jsonData);

        private:
            void lex_string(std::string_view jsonData, long *start, long *end);
            void lex_bool(std::string_view jsonData, long *start, long *end);
            void lex_null(std::string_view jsonData, long *start, long *end);
            void lex_number(std::string_view jsonData, long *start, long *end);
        };
    }

} // namespace utils

#endif // __LEXER_H__