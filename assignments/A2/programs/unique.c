#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

int main() {
    // Initialize buffer
    char buffer[BUFFER_SIZE];
    char prev[BUFFER_SIZE] = "\0";

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Print string iff the previous string is different
        if (strncmp(buffer, prev, BUFFER_SIZE) != 0) {
            printf("%s", buffer);
        }

        // Save word for next iteration
        strncpy(prev, buffer, BUFFER_SIZE);
    }

    return 0;
}