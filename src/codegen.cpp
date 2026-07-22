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

    size_t pc_slot = 0;

    auto emit = [&](const std::string& instr, bool has_arg) {
        code.push_back(instr);
        pc_slot += has_arg ? 2 : 1;
    };

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
                        emit("PUSH, " + std::to_string(static_cast<int>(tokens[count].value[i])) + ",", true);
                    }
                    for (int i = len - 1; i >= 0; --i) {
                        emit("COUT,", false);
                        emit("POP,", false);
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
                emit("PUSH, " + tokens[count].value + ",", true);
                count++;
            } else {
                emit("PUSH, 0,", true);
            }

            if (count < tokens.size() && tokens[count].type == TokenType::token_eqeq) {
                count++;
            }

            if (count < tokens.size() && tokens[count].type == TokenType::token_number) {
                emit("PUSH, " + tokens[count].value + ",", true);
                count++;
            } else {
                emit("PUSH, 0,", true);
            }

            emit("EQ,", false);

            unresolved_jumps.push_back(code.size());
            emit("JIZ, {PENDENT},", true);

            continue;

        } else if (actual.type == TokenType::keyword_end) {
            count++;

            if (!unresolved_jumps.empty()) {
                size_t jump_idx = unresolved_jumps.back();
                unresolved_jumps.pop_back();

                size_t target_slot = pc_slot;

                code[jump_idx] = "JIZ, " + std::to_string(target_slot) + ",";
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