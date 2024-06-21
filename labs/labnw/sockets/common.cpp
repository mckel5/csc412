#include "common.h"

#include <sys/socket.h>

#include <iostream>

/**
 * Validate the supplied arguments.
 * Neither the client nor the server take any arguments, so supplying them causes the program to exit.
*/
void checkargs(int argc, const char *argv[]) {
    if (argc > 1) {
        std::cerr << "You naughty little elf, this program takes no command-line arguments. :/" << std::endl;
        for (int i = 0; i < argc; ++i) {
            std::cout << "\targument " << i << ": " << argv[i] << std::endl;
        }
        exit(1);
    }
}

/**
 * Print an error to stdout, then exit.
*/
void pserror(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Return a new socket.
 * Raises an error if the socket was not created successfully.s
*/
int initialize_socket() {
    int obj_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (obj_socket < 0) {
        pserror("Socket creation error");
    }
    return obj_socket;
}