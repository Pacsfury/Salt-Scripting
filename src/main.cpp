#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "../include/tokens.hpp"
#include "../include/codegen.hpp"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::token_lparen:   return "LEFT_PAREN";
        case TokenType::token_rparen:   return "RIGHT_PAREN";
        case TokenType::token_l_string: return "STRING_LITERAL";
        case TokenType::token_name:     return "IDENTIFIER";
        case TokenType::keyword_print:  return "KEYWORD_PRINT";
        default:                        return "UNKNOWN";
    }
}

int main() {
    std::string input = "print(\"hello world\")";
    std::vector<Token> tokens = tokenize(input);

    for (const auto& token : tokens) {
        std::cout << "Type: " << tokenTypeToString(token.type)
                  << ", Value: \"" << token.value << "\"\n";
    }

    generate(std::move(tokens));

    return 0;
}


// cl src/main.cpp src/*.cpp /EHsc