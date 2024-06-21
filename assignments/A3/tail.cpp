#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

void print_tail(std::istream& stream);

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
            print_tail(file);

            // Separate files by newline
            if (i != argc - 1) std::cout << std::endl;
        }
    } else {
        // Build a queue that stores the most recent 10 lines from stdin
        // Can't use print_tail() since std::cin does not support seeking
        std::queue<std::string> q;
        const int max_lines = 10;

        std::string line;
        while (std::getline(std::cin, line)) {
            q.push(line);
            // Limit size of queue
            if (q.size() > max_lines) q.pop();
        }

        // Print each line
        for (int i = 0; i < max_lines && !q.empty(); i++) {
            std::string line = q.front();
            std::cout << line << std::endl;
            q.pop();
        }
    }

    return error;
}

/**
 * Print the last 10 lines of a stream.
 */
void print_tail(std::istream& stream) {
    char c;
    std::stringstream buffer;
    int newline_count = 0;
    bool start_reached = false;

    // Seek to end of stream
    stream.seekg(-1, std::ios::end);

    // Don't count trailing newline for the total count
    stream.get(c);
    if (c != '\n') buffer << c;
    stream.seekg(-2, std::ios::cur);

    while (!start_reached) {
        // If cursor is at position 0, we are at the start
        if (stream.tellg() == 0) start_reached = true;

        // get() automatically advances the cursor by one
        stream.get(c);
        if (c == '\n') newline_count++;
        if (newline_count == 10) break;
        buffer << c;

        // Back up by two to account for the call to get()
        stream.seekg(-2, std::ios::cur);
    }

    // Reverse the buffer
    std::string s = buffer.str();
    std::string reversed;
    std::copy(s.crbegin(), s.crend(), std::back_inserter(reversed));

    std::cout << reversed << std::endl;
}