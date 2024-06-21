#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

/* Split a char buffer into a series of words, separated by newlines */
char* makewords(const char* input) {
    // calculate the maximum possible len of the result
    int maxLen = strlen(input) + 1;

    char* result = (char*)malloc(maxLen);  // allocate memory for the result
    if (result == NULL) {
        fprintf(stderr, "ERROR: memory allocation failed in makewords.\n");
        exit(1);
    }

    // Manually copying the first character eliminates the need for bounds checking in the for loop
    result[0] = input[0];

    for (int i = 1, result_index = 1; i < maxLen; i++) {
        // Skip over repeated spaces (result_index does not get incremented)
        if (input[i] == ' ' && input[i - 1] == ' ') continue;

        if (input[i] == ' ') {
            result[result_index++] = '\n';
        } else {
            result[result_index++] = input[i];
        }

        if (input[i] == '\0') break;
    }

    // null-terminate the final spot in the result, leave as is
    result[maxLen - 1] = '\0';

    return result;
}

int main() {
    // Initialize buffer
    char buffer[BUFFER_SIZE];

    // Read data from stdin and write it to stdout (standard output)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // pass data from the buffer to a function convert spaces to linebreaks
        char* result = makewords(buffer);
        fputs(result, stdout);
        free(result);  // releases the memory allocated
    }

    return 0;
}
