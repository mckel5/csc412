#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "common.h"

int main(int argc, const char *argv[]) {
    checkargs(argc, argv);

    int reader;
    struct sockaddr_in serv_addr;
    const char *message = "CLIENT: I am sending a message!";
    char buffer[1024] = {0};

    // Initialize server socket
    int obj_socket = initialize_socket();

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    const char *ip_address = "127.0.0.1";

    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        pserror("Invalid address");
    }

    // Connect to server
    if (connect(obj_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        pserror("Connection failed");
    }

    // Send message to server
    send(obj_socket, message, strlen(message), 0);
    std::cout << "CLIENT: Message has been sent!" << std::endl;

    // Receive server response
    reader = read(obj_socket, buffer, 1024);
    if (reader > 0) {
        std::cout << "CLIENT: printing the buffer..." << std::endl;
        std::cout << buffer << std::endl;
    }

    // Close socket
    close(obj_socket);

    return 0;
}
