#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

void print_summary(std::istream& stream);
bool is_whitespace(char);

bool error = false;
int total_lines = 0;
int total_words = 0;
int total_bytes = 0;

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        // Evaluate files

        for (int i = 1; i < argc; i++) {
            char* filename = argv[i];

            // Handle nonexistent files
            if (!std::filesystem::exists(filename)) {
                std::cerr << argv[0] << ": " << filename << ": No such file or directory" << std::endl;
                error = true;
                continue;
            }

            std::ifstream file(filename);
            print_summary(file);

            std::cout << filename << std::endl;
        }

        // Print total counts
        if (argc > 2)
            std::cout << total_lines << " " << total_words << " " << total_bytes << " total" << std::endl;

    } else {
        // Evaluate stdin

        print_summary(std::cin);

        std::cout << "(stdin)" << std::endl;
    }

    return error;
}

/**
 * Print the number of lines, bytes, and words in a stream.
 *
 * A line is defined as an instance of a newline character ('\n').
 * A word is defined as whitespace followed by any non-whitespace character.
 */
void print_summary(std::istream& stream) {
    int lines = 0;
    int words = 0;
    int bytes = 0;

    // Load file into string
    std::stringstream buffer;
    buffer << stream.rdbuf();
    std::string stream_contents = buffer.str();

    for (size_t i = 0; i < stream_contents.length(); i++) {
        bytes++;

        if (stream_contents[i] == '\n') lines++;

        // Account for final word in file
        if (i + 1 == stream_contents.length()) {
            words++;
            break;
        }

        if (is_whitespace(stream_contents[i]) && !is_whitespace(stream_contents[i + 1])) words++;
    }

    total_lines += lines;
    total_words += words;
    total_bytes += bytes;

    std::cout << lines << " " << words << " " << bytes << " ";
}

bool is_whitespace(char c) {
    return c == ' ' ||
           c == '\n' ||
           c == '\t' ||
           c == '\r';
}