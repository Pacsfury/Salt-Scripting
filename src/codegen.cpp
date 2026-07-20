#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "../include/tokens.hpp"
#include "../include/codegen.hpp"

void generate(std::vector<Token> tokens) {
    std::vector<std::string> code;
    std::vector<size_t> unresolved_jumps; 
    
    size_t count = 0;
    
    while (count < tokens.size()) {
        
        Token actual = tokens[count]; 

        if (actual.type == TokenType::token_EOF) {
            count++;
            continue;
        }

        if (actual.type == TokenType::keyword_print) {
            if (count + 1 < tokens.size() && tokens[count + 1].type == TokenType::token_lparen) {
                count += 2;

                if (count < tokens.size() && tokens[count].type == TokenType::token_l_string) {
                    int len = static_cast<int>(tokens[count].value.length());
                    
                    for (int i = len - 1; i >= 0; --i) {
                        code.push_back("PUSH, " + std::to_string(static_cast<int>(tokens[count].value[i])) + ",");
                    }
                    for (int i = len - 1; i >= 0; --i) {
                        code.push_back("COUT,");
                        code.push_back("POP,");
                    }
                    
                    count++;
                }
                
                if (count < tokens.size() && tokens[count].type == TokenType::token_rparen) {
                    count++; 
                }
                
                continue;
            }
        } else if (actual.type == TokenType::keyword_if) {
            count++;

            if (count < tokens.size() && tokens[count].type == TokenType::token_number) {
                code.push_back("PUSH, " + tokens[count].value + ",");
                count++;
            } else {
                code.push_back("PUSH, 0,");
            }

            if (count < tokens.size() && tokens[count].type == TokenType::token_eqeq) {
                count++;
            }

            if (count < tokens.size() && tokens[count].type == TokenType::token_number) {
                code.push_back("PUSH, " + tokens[count].value + ",");
                count++;
            } else {
                code.push_back("PUSH, 0,");
            }

            code.push_back("EQ,");

            unresolved_jumps.push_back(code.size());
            code.push_back("JIZ, {PENDENT},");

            continue;

        } else if (actual.type == TokenType::keyword_end) {
            count++;

            if (!unresolved_jumps.empty()) {
                size_t jump_idx = unresolved_jumps.back();
                unresolved_jumps.pop_back();

                size_t target_instruction = code.size();

                size_t compensated = (target_instruction == 0) ? 0 : target_instruction - 1;

                code[jump_idx] = "JIZ, " + std::to_string(compensated) + ",";
            }

            continue;
        }
        
        count++; 
    }

    std::ofstream output("program.gosb");
    if (!output.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }

    for (const std::string& instruction : code) {
        output << instruction << "\n";
    }

    output.close();

    std::system("vm.exe");
}