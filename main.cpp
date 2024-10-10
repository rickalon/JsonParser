#include <string_view>
#include <iostream>
#include <array>
#include "Parser.h"

bool parseAndPrint(std::string_view json)
{
   Parser parser(json);

    std::cout << "Starting parsing of: " << json << std::endl;
    
    auto ast = parser.getAST();

    std::cout << "Traversing the AST:" << std::endl;
    parser.traverseAST(ast.get());
    
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
