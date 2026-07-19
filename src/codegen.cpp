#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "../include/tokens.hpp"
#include "../include/codegen.hpp"

void generate(std::vector<Token> tokens) {
    std::ofstream output("program.gosb");

    if (!output.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    size_t count = 0;
    
    while (count < tokens.size() && tokens[count].type != TokenType::token_EOF) {
        
        Token actual = tokens[count]; 

        if (actual.type == TokenType::keyword_print) {
            if (count + 1 < tokens.size() && tokens[count + 1].type == TokenType::token_lparen) {
                count += 2;

                if (count < tokens.size() && tokens[count].type == TokenType::token_l_string) {
                    int len = static_cast<int>(tokens[count].value.length());
                    
                    for (int i = len - 1; i >= 0; --i) {
                        output << "PUSH," << static_cast<int>(tokens[count].value[i]) << ", ";
                    }
                    for (int i = len - 1; i >= 0; --i) {
                        output << "COUT," << "POP, ";
                    }
                    
                    count++;
                }
                
                if (count < tokens.size() && tokens[count].type == TokenType::token_rparen) {
                    count++; 
                }
                
                output << "\n"; 
                continue;
            }
        }
        
        count++; 
    }

    output.close();

    std::system("vm.exe");
}