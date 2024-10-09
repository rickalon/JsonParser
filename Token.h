#ifndef TOKEN_H
#define TOKEN_H

#include <string_view>

enum class TokenKind
{
    LBraceKey = 0,
    RBraceKey,
    LSquareKey,
    RSquareKey,
    Colon,
    Comma,
    Number,
    String,
    Bool,
    Null,
    EndOfFile,
};

struct Token
{
    std::string_view lexem;
    TokenKind kind;
};

#endif