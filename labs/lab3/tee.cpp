#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    bool file_specified = argc >= 2;
    std::ofstream file;

    if (file_specified) {
        file.open(argv[1]);
        file.clear();
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
        if (file_specified) file << line << std::endl;
    }

    if (file_specified) file.close();

    return 0;
}