#include <bits/stdc++.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

std::string logDir = "logFolder/";
std::string outputFile = logDir + "actions.txt";

int main() {
    // Check if log directory exists
    if (!fs::exists(logDir)) {
        std::cerr << logDir << " directory does not exist\n";
        return 1;
    }

    // Remove actions.txt if it already exists
    if (fs::exists(outputFile)) fs::remove(outputFile);

    std::vector<std::string> lines;

    // Load each log entry into memory
    for (const auto& file : fs::directory_iterator(logDir)) {
        std::ifstream stream(file.path());

        std::string line;
        while (std::getline(stream, line)) {
            lines.push_back(line);
        }
    }

    // Sort the lines alphabetially
    sort(lines.begin(), lines.end());

    std::ofstream file(outputFile);

    // Write to the combined log file
    for (const auto& line : lines) {
        file << line << '\n';
    }

    // Set 755 permissions on the combined log file
    fs::permissions(
        outputFile,
        fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read | fs::perms::others_exec,
        fs::perm_options::replace);

    return 0;
}
