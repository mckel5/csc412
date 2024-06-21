#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

bool error = false;

int main(int argc, char* argv[]) {
    std::vector<std::ofstream> files;

    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        
        // Determine parent directory of input file
        fs::path relative(filename);
        fs::path absolute = fs::absolute(relative);
        absolute.remove_filename();

        // Handle nonexistent parent directory
        if (!std::filesystem::exists(absolute)) {
            // tee errors must not include the program path for some reason!
            std::cerr << "Error: Could not open file " << filename << "." << std::endl;
            error = true;
            continue;
        }

        files.emplace_back(std::ofstream{filename});
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        // Send to stdout
        std::cout << line << std::endl;

        // Send to file(s)
        for (auto& file : files) {
            file << line << std::endl;
        }
    }

    return error;
}