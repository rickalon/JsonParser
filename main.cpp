#include <array>
#include <iostream>
#include <map>
#include <stack>
#include <string_view>
#include <vector>

enum class ValueType
{
    VECTOR,
    STRING,
    BOOL,
    ENT,
    DIC,
    KEY,
};

class Node
{
    Node *value;
    std::string key;
    ValueType type;
};

Node *parseJson(std::string_view json)
{
    std::stack<char> st;
    Node aux;
    std::string str;
    std::vector<std::string> strAux;
    std::map<std::string,Node> dic; 
    for (const char &c : json)
    {
        if (c == '{' || c == '[' || c == '\"')
        {
            st.push(c);
        }

        if (st.top() == '{' && c == '}')
        {
            std::cout << "final" << std::endl;
            st.pop();
            strAux.push_back(str);
            str = "";
            continue;
        }
        else if (st.top() == '{' && (c == ']' && c == '\"'))
        {
            return nullptr;
        }
        else if (st.top() == '[' && c == ']')
        {
            st.pop();
            strAux.push_back(str);
            str = "";
            continue;
        }
        else if (st.top() == '[' && (c == '}' && c == '\"'))
        {
            return nullptr;
        }
        else if (st.top() == '\"' && c == '\"')
        {
            strAux.push_back(str);
            str = "";
            st.pop();
            continue;
        }
        else if (st.top() == '\"' && (c == '}' && c == ']'))
        {
            return nullptr;
        }

        if (c == ':')
        {
            strAux.push_back(str);
            str = "";
        }
        else if (c == ',')
        {
            strAux.push_back(str);
            str = "";
        }
        else
        {
            str += c;
        }
        

    }
    if (!st.empty())
    {
        return nullptr;
    }
    std::cout << "json:" << json << std::endl;
    for(std::string& val: strAux){
        std::cout << val << std::endl;
    }
    return new Node();
}

bool parseAndPrint(std::string_view json)
{
    std::cout << json << std::endl;
    return true;
}

int main()
{
    parseJson("{ \"name\": \"John Doe\", \"age\": 30, \"isStudent\": false }");
    //   static constexpr std::array<std::string_view, 8> validTests = {
    //       "{}",
    //       "\"\"",
    //       "\"string\"",
    //       "10",
    //       "true",
    //       "[]",
    //       "{ \"key\" : \"val\" }",
    //       "[10, true, []]"};

    //   for (std::string_view test : validTests) {
    //     if (!parseAndPrint(test)) {
    //       std::cerr << "Failed parsing: " << test << "\n";
    //       return -1;
    //     }
    //   }

    return 0;
}
