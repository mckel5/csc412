#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

int main(int, char* argv[]) {
    fs::path pwd;

    try {
        pwd = fs::current_path();
    } catch (const fs::filesystem_error& e) {
        std::cerr << argv[0] << ": error: No such file or directory" << std::endl;
        return 1;
    }

    std::cout << pwd.string() << std::endl;
    return 0;
}
