#include <array>
#include <iostream>
#include <map>
#include <stack>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>

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

            if(n == std::string_view::npos){
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
            constexpr size_t NULL_SIZE = 4;
            return genTokenAndConsume(TokenKind::Null, NULL_SIZE);
        }
        case '"':
        {
            auto endOfString = jsonStr.find_first_of("\"", 3);
            return genTokenAndConsume(TokenKind::String, endOfString);
        }
        case 't':
        {
            constexpr size_t TRUE_SIZE = 4;
            return genTokenAndConsume(TokenKind::Bool, TRUE_SIZE);
        }
        case 'f':
        {
            constexpr size_t FALSE_SIZE = 5;
            return genTokenAndConsume(TokenKind::Bool, FALSE_SIZE);
        }
        case '0' ... '9':
        {
            auto endOfNumber = jsonStr.find_first_not_of("0123456789",0);
            return genTokenAndConsume(TokenKind::Number, endOfNumber);
        }
        }
        __builtin_unreachable();
    }

private:
    std::string_view jsonStr;
};

bool parseAndPrint(std::string_view json)
{
    Lexer lex{json};

    std::cout << "Starting parsing of: " << json << std::endl;

    Token token = lex.getNextToken();
    while (token.kind != TokenKind::EndOfFile)
    {
        std::cout << "Got token: " << token.lexem << std::endl;
        token = lex.getNextToken();
    }

    return true;
}

int main()
{
    static constexpr std::array<std::string_view, 8> validTests = {
        "{}",
        "\"\"",
        "\"string\"",
        "110",
    "true",
    "[]",
    "{ \"key\" : \"val\" }",
    "[10, true, []]"};

    for (std::string_view test : validTests)
    {
        if (!parseAndPrint(test))
        {
            std::cerr << "Failed parsing: " << test << "\n";
            return -1;
        }
    }

    return 0;
}
