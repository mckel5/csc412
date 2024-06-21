#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

void print_head(std::istream&);

bool error = false;

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            char* filename = argv[i];

            // Handle nonexistent files
            if (!std::filesystem::exists(filename)) {
                std::cerr << argv[0] << ": cannot open '" << filename << "' for reading: No such file or directory" << std::endl;
                error = true;
                continue;
            }

            // Print separators when multiple files specified
            if (argc > 2) std::cout << "==> " << filename << " <==" << std::endl;

            std::ifstream file(filename);
            print_head(file);
            
            // Separate files by newline
            if (i != argc - 1) std::cout << std::endl;
        }
    } else {
        print_head(std::cin);
    }

    return error;
}

/**
 * Print the first 10 lines of a stream.
 */
void print_head(std::istream& stream) {
    int i = 0;
    std::string line;
    while (std::getline(stream, line) && i < 10) {
        std::cout << line << std::endl;
        i++;
    }
}