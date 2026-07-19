#pragma once

#include <string>
#include <vector>

enum class TokenType  {
    token_rparen,
    token_lparen,
    token_l_string,
    token_name,
    keyword_print
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(std::string line);