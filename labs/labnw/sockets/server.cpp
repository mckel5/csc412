#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "common.h"

int main(int argc, const char *argv[]) {
    checkargs(argc, argv);

    int server_socket, client_socket;
    struct sockaddr_in address;
    int opted = 1;
    socklen_t address_length = sizeof(address);
    char buffer[1024] = {0};
    const char *message = "Hi I am a message from the server!";

    // Initialize server socket
    server_socket = initialize_socket();

    // Set socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opted, sizeof(opted))) {
        pserror("Can't set the socket");
    }

    // Configure socket address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        pserror("Binding of socket failed !");
    }

    // Listen for clients
    if (listen(server_socket, 3) < 0) {
        pserror("Can't listen from the server !");
    }

    // Accept client connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&address, &address_length)) < 0) {
        pserror("Accept");
    }

    // Receive client message
    ssize_t reader = read(client_socket, buffer, 1024);
    if (reader > 0) {
        std::cout << buffer << std::endl;
    }

    // Send response to client
    send(client_socket, message, strlen(message), 0);
    std::cout << "SERVER: I just sent a message to the client!" << std::endl;

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
