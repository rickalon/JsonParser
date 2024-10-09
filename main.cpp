#include <string_view>
#include <iostream>
#include <array>
#include "Lexer.h"

bool parseAndPrint(std::string_view json)
{
   Lexer lex(json);

    std::cout << "Starting parsing of: " << json << std::endl;

    Token token = lex.getNextToken();
    while (token.kind != TokenKind::EndOfFile)
    {
        std::cout << "Got token: " << token.lexem << std::endl;
        //par.putToken(token);
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