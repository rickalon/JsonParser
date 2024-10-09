#ifndef LEXER_H
#define LEXER_H

#include <string_view>
#include "Token.h"

class Lexer
{
public:
    Lexer(std::string_view jsonStr) : jsonStr(jsonStr) {}

    Token getNextToken()
    {
        auto lTrim = [](std::string_view &str)
        {
            auto isDroppableChar = [](char c)
            {
                switch (c)
                {
                case ' ':
                case '\t':
                case '\n':
                    return true;
                default:
                    return false;
                }
            };

            while (!str.empty() && isDroppableChar(str.front()))
            {
                str = str.substr(1);
            }
        };
        lTrim(jsonStr);

        if (jsonStr.empty())
        {
            return Token{"", TokenKind::EndOfFile};
        }

        auto genTokenAndConsume = [&](TokenKind kind, size_t n = 1)
        {
            if (n == std::string_view::npos)
            {
                auto lex = jsonStr;
                jsonStr = std::string_view();
                return Token{lex, kind};
            }
            auto lex = jsonStr.substr(0, n);
            jsonStr = jsonStr.substr(n);
            return Token{lex, kind};
        };

        const char c = jsonStr.front();
        switch (c)
        {
        case '{':
            return genTokenAndConsume(TokenKind::LBraceKey);
        case '}':
            return genTokenAndConsume(TokenKind::RBraceKey);
        case ']':
            return genTokenAndConsume(TokenKind::RSquareKey);
        case '[':
            return genTokenAndConsume(TokenKind::LSquareKey);
        case ',':
            return genTokenAndConsume(TokenKind::Colon);
        case ':':
            return genTokenAndConsume(TokenKind::Comma);
        case 'n':
        {
            if (jsonStr.at(0) == 'n' && jsonStr.at(1) == 'u' && jsonStr.at(2) == 'l' && jsonStr.at(3) == 'l')
            {
                constexpr size_t NULL_SIZE = 4;
                return genTokenAndConsume(TokenKind::Null, NULL_SIZE);
            }
        }
        case '"':
        {
            auto endOfString = jsonStr.find_first_of("\"", 1);
            return genTokenAndConsume(TokenKind::String, ++endOfString);
        }
        case 't':
        {
            if (jsonStr.at(1) == 'r' && jsonStr.at(2) == 'u' && jsonStr.at(3) == 'e')
            {
                constexpr size_t TRUE_SIZE = 4;
                return genTokenAndConsume(TokenKind::Bool, TRUE_SIZE);
            }
        }
        case 'f':
        {
            if (jsonStr.at(0) == 'f' && jsonStr.at(1) == 'a' && jsonStr.at(2) == 'l' && jsonStr.at(3) == 's' && jsonStr.at(4) == 'e')
            {
                constexpr size_t FALSE_SIZE = 5;
                return genTokenAndConsume(TokenKind::Bool, FALSE_SIZE);
            }
        }
        case '0' ... '9':
        {
            auto endOfNumber = jsonStr.find_first_not_of("0123456789", 0);
            return genTokenAndConsume(TokenKind::Number, endOfNumber);
        }
        }
        __builtin_unreachable();
    }

private:
    std::string_view jsonStr;
};

#endif