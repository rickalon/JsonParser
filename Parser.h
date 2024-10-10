#ifndef PARSER_H
#define PARSER_H

    // Start           => JsonValue EOF
    // JsonValue       => Dictionary|Array|Null|Bool|String|Number
    // Dictionary      => { (DictionaryEntry (, DictionaryEntry)*)? }
    // DictionaryEntry => String : JsonValue
    // Array           => [(JsonValue (, JsonValue)*)?]
    // Null            => null
    // Bool            => true|false
    // String          => ".*"
    // Number          => [0-9]+
    // EOF             => $

    #include "Token.h"
    #include "Lexer.h"
    #include <string_view>
    #include <string>
    #include <map>
    #include <memory>
    #include <vector>
    #include <iostream>
    #include "Lexer.h"

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct DictionaryNode : public ASTNode {
    std::map<std::string, std::unique_ptr<ASTNode>> members;
};


struct ArrayNode : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> elements;
};

struct LiteralNode : public ASTNode {
    enum class LiteralType { String, Number, Boolean, Null };
    LiteralType type;
    std::string value;
};

class Parser {
public:
    Parser(std::string_view json) : lex(json) {
        getToken();
    }

    std::unique_ptr<ASTNode> getAST() {
        return parseValue(); 
    }

    void traverseAST(const ASTNode* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    if (auto dictNode = dynamic_cast<const DictionaryNode*>(node)) {
        std::cout << "DictionaryNode {" << std::endl;
        for (const auto& [key, value] : dictNode->members) {
            for (int i = 0; i < depth + 1; ++i) {
                std::cout << "  ";
            }
            std::cout << "\"" << key << "\": " << std::endl;
            traverseAST(value.get(), depth + 2);
        }
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        std::cout << "}" << std::endl;
    } else if (auto arrayNode = dynamic_cast<const ArrayNode*>(node)) {
        std::cout << "ArrayNode [" << std::endl;
        for (const auto& element : arrayNode->elements) {
            traverseAST(element.get(), depth + 1);
        }
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        std::cout << "]" << std::endl;
    } else if (auto literalNode = dynamic_cast<const LiteralNode*>(node)) {
        std::cout << "LiteralNode (" << literalNode->value << ")" << std::endl;
    } else {
        std::cout << "Unknown ASTNode" << std::endl;
    }
}

private:
    void getToken() {
        currentToken = lex.getNextToken();
    }

    std::unique_ptr<ASTNode> parseValue() {
        switch (currentToken.kind) {
            case TokenKind::LBraceKey:
                return parseObject();
            case TokenKind::LSquareKey:
                return parseArray();
            case TokenKind::String:
            case TokenKind::Number:
            case TokenKind::Bool:
            case TokenKind::Null:
                return parseLiteral();
            default:
                throw std::runtime_error("Unexpected token");
        }
    }

    std::unique_ptr<ASTNode> parseObject() {
        auto node = std::make_unique<DictionaryNode>();
        getToken();

        while (currentToken.kind != TokenKind::RBraceKey) {
            if (currentToken.kind != TokenKind::String) {
                throw std::runtime_error("Expected a string key");
            }

            std::string key = std::string(currentToken.lexem);
            getToken(); 

            if (currentToken.kind != TokenKind::Colon) {
                throw std::runtime_error("Expected ':' after key");
            }

            getToken(); 

            node->members[key] = parseValue();

            if (currentToken.kind == TokenKind::Comma) {
                getToken();
            } else if (currentToken.kind != TokenKind::RBraceKey) {
                throw std::runtime_error("Expected ',' or '}'");
            }
        }

        getToken(); 
        return node;
    }

    std::unique_ptr<ASTNode> parseArray() {
        auto node = std::make_unique<ArrayNode>();
        getToken();

        while (currentToken.kind != TokenKind::RSquareKey) {
            node->elements.push_back(parseValue());

            if (currentToken.kind == TokenKind::Comma) {
                getToken(); 
            } else if (currentToken.kind != TokenKind::RSquareKey) {
                throw std::runtime_error("Expected ',' or ']'");
            }
        }

        getToken(); 
        return node;
    }

    std::unique_ptr<ASTNode> parseLiteral() {
        auto node = std::make_unique<LiteralNode>();
        
        if (currentToken.kind == TokenKind::String) {
            node->type = LiteralNode::LiteralType::String;
        } else if (currentToken.kind == TokenKind::Number) {
            node->type = LiteralNode::LiteralType::Number;
        } else if (currentToken.kind == TokenKind::Bool) {
            node->type = LiteralNode::LiteralType::Boolean;
        } else if (currentToken.kind == TokenKind::Null) {
            node->type = LiteralNode::LiteralType::Null;
        }

        node->value = currentToken.lexem;
        getToken(); 
        return node;
    }

    Token currentToken;
    Lexer lex;
};

#endif
