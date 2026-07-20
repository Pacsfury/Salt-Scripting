#include <string>
#include <vector>
#include <cctype>
#include "../include/tokens.hpp"

std::vector<Token> tokenize(std::string line) {
    std::vector<Token> tokenarray;
    int current = 0;

    while (line[current] != '\0') {
        // Ignorem espais, tabulacions i salts de línia
        if (line[current] == ' ' || line[current] == '\n' || line[current] == '\r' || line[current] == '\t') {
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
            case '=':
                // Comprovem si el següent caràcter és '=' per formar '=='
                if (line[current + 1] == '=') {
                    newtoken.type = TokenType::token_eqeq;
                    current++; // Saltem el segon '='
                }
                break;
            case '"': {
                int actl = current + 1;
                std::string buffer = "";
                while (line[actl] != '"' && line[actl] != '\0') {
                    if (line[actl] == '\\' && line[actl + 1] == 'n') {
                        buffer += '\n';
                        actl += 2; 
                    } else {
                        buffer += line[actl++];
                    }
                }
                newtoken.type = TokenType::token_l_string;
                newtoken.value = buffer;
                if (line[actl] == '"') current = actl;
                break;
            }
            case '\'': {
                int actl = current + 1;
                std::string buffer = "";
                while (line[actl] != '\'' && line[actl] != '\0') {
                    if (line[actl] == '\\' && line[actl + 1] == 'n') {
                        buffer += '\n';
                        actl += 2; 
                    } else {
                        buffer += line[actl++];
                    }
                }
                newtoken.type = TokenType::token_l_string;
                newtoken.value = buffer;
                if (line[actl] == '\'') current = actl;
                break;
            }   
            default: {
                int actl = current;
                std::string buffer = "";

                // Delimitem la paraula aturant-nos en qualsevol separador o control de línia
                while (line[actl] != ' '  && line[actl] != '\0' &&
                       line[actl] != '\n' && line[actl] != '\r' && line[actl] != '\t' &&
                       line[actl] != '('  && line[actl] != ')'  &&
                       line[actl] != '"'  && line[actl] != '\'') {
                    buffer += line[actl++];
                }
                
                if (buffer == "print") {
                    newtoken.type = TokenType::keyword_print;
                } else if (buffer == "if") {
                    newtoken.type = TokenType::keyword_if;
                } else if (buffer == "end") {
                    newtoken.type = TokenType::keyword_end;                   
                } else {
                    bool is_number = !buffer.empty();
                    for (char c : buffer) {
                        if (!std::isdigit(c)) {
                            is_number = false;
                            break;
                        }
                    }

                    if (is_number) {
                        newtoken.type = TokenType::token_number;
                        newtoken.value = buffer;
                    } else {
                        newtoken.type = TokenType::token_name;
                        newtoken.value = buffer;
                    }
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

    Token eofToken;
    eofToken.type = TokenType::token_EOF;
    tokenarray.push_back(eofToken);
    return tokenarray;
}