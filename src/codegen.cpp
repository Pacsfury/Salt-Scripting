#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "../include/tokens.hpp"
#include "../include/codegen.hpp"

void generate(std::vector<Token> tokens) {
    std::ofstream output("output.txt");

    if (!output.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    int count = 0;
    Token actual = tokens[count];

    if (actual.type == TokenType::keyword_print) {
        if (tokens[++count].type == TokenType::token_lparen) {
            count++;

            if (tokens[count].type == TokenType::token_l_string) {
                int len = static_cast<int>(tokens[count].value.length());
                for (int i = len - 1; i >= 0; --i) {
                    output << "PUSH," << static_cast<int>(tokens[count].value[i]) << ", ";
                }
                for (int i = len - 1; i >= 0; --i) {
                    output << "COUT," << "POP, ";
                }                
            }
        }
    }


    output.close();
}