#include <iostream>
#include <unistd.h> // For getcwd
#include <cstring> // For strerror
#include <cerrno> // For errno

int main(int argc, char* argv[]) {
    char cwd[256]; // Allocate a buffer to hold the current working directory path

    // Attempt to get the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // On success, print the current working directory to the standard output
        if(argc == 1){
            std::cout << cwd << std::endl;
        }
    } else {
        // When getcwd fails, print a custom error message using the program's name (argv[0])
        // and the specific error message for the current value of errno
        
        std::cerr << argv[0] << ": error: " << strerror(errno) << std::endl;
        
        return 1; // Return an error code to indicate failure
    }

    return 0; // Return 0 to indicate success
}