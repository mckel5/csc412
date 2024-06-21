#include <iostream>
#include <unistd.h> // For getcwd
#include <cstring> // For strerror
#include <cerrno> // For errno
#include <vector> // For dynamically resizable array

int main(int argc, char* argv[]) {
    std::vector<char> cwdBuffer(256); // Start with a reasonable size
    
    while (true) {
        // Attempt to get the current working directory
        if (getcwd(cwdBuffer.data(), cwdBuffer.size()) != NULL) {
            // Success, break out of the loop
            break;
        } else if (errno == ERANGE) {
            // Buffer size is too small; increase it and try again
            cwdBuffer.resize(cwdBuffer.size() * 2);
        } else {
            // An error other than ERANGE occurred; print an error message and exit
            std::cerr << argv[0] << ": error: " << strerror(errno) << std::endl;
            return 1; // Return an error code to indicate failure
        }
    }

    // On success, print the current working directory to the standard output
    if (argc == 1) {
        std::cout << cwdBuffer.data() << std::endl;
    }


    return 0; // Return 0 to indicate success
}