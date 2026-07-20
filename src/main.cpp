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

    std::string line;
    std::vector<Token> all_tokens;

    while (std::getline(file, line)) {
        if (line.empty()) continue; 

        std::vector<Token> line_tokens = tokenize(line);
        all_tokens.insert(all_tokens.end(), line_tokens.begin(), line_tokens.end());
    }

    file.close();

    std::cout << "------ VM EXECUTION ------\n";
    generate(all_tokens); 

    return 0;
}

// cl /std:c++17 src/main.cpp src/*.cpp /EHsc ; ./main run main.salt