#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "../include/tokens.hpp"
#include "../include/codegen.hpp"
#include "../include/argc.hpp"

int main(int argc, char* argv[]) {
    ArgsContext args(argc, argv);
    
    auto filename_opt = args.getArg("run");
    if (!filename_opt.has_value()) {
        std::cerr << "Error: Missing input file.\n";
        return 1;
    }

    std::ifstream file((std::string(filename_opt.value())));
    if (!file) {
        std::cerr << "Error: Couldn't open file.\n";
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    generate(tokenize(input)); 

    return 0;
}