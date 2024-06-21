#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

int main() {
    // Initialize buffer
    char buffer[BUFFER_SIZE];

    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Make each char lowercase, if applicable
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = tolower(buffer[i]);
        }

        fputs(buffer, stdout);
    }

    return 0;
}