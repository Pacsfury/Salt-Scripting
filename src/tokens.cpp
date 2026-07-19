#include <string>
#include <vector>
#include "../include/tokens.hpp"

std::vector<Token> tokenize(std::string line) {
    std::vector<Token> tokenarray;

    int current = 0;

    while (line[current] != '\0') {
        if (line[current] == ' ') {
            current++;
            continue;
        }

        char src = line[current];
        Token newtoken;
        switch (src) {
            case '(':
                newtoken.type = TokenType::token_lparen;
                break;
            case ')':
                newtoken.type = TokenType::token_rparen;
                break;
            case '"': {
                int actl = current + 1;
                std::string buffer = "";
                while (line[actl] != '"' && line[actl] != '\0') {
                    buffer += line[actl++];
                }
                newtoken.type = TokenType::token_l_string;
                newtoken.value = buffer;
                if (line[actl] == '"') {
                    current = actl;
                }
                break;
            }
            case '\'': {
                int actl = current + 1;
                std::string buffer = "";
                while (line[actl] != '\'' && line[actl] != '\0') {
                    buffer += line[actl++];
                }
                newtoken.type = TokenType::token_l_string;
                newtoken.value = buffer;
                if (line[actl] == '\'') {
                    current = actl;
                }
                break;
            }
            default: {
                int actl = current;
                std::string buffer = "";
                while (line[actl] != ' ' && line[actl] != '\0' &&
                       line[actl] != '(' && line[actl] != ')' &&
                       line[actl] != '"' && line[actl] != '\'') {
                    buffer += line[actl++];
                }
                
                if (buffer == "print") {
                    newtoken.type = TokenType::keyword_print;
                } else {
                    newtoken.type = TokenType::token_name;
                    newtoken.value = buffer;
                }

                if (actl > current) {
                    current = actl - 1;
                }
                break;
            }
        }
        tokenarray.push_back(newtoken);

        current++;
    }
    return tokenarray;
}