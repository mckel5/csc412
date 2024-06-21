#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

// Compare two strings alphabetically
// https://www.geeksforgeeks.org/c-program-sort-array-names-strings/
static int alphabetical_compare(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int main() {
    // Initialize buffer
    char buffer[BUFFER_SIZE];
    // Allocating full BUFFER_SIZE results in a segfault, so divide in half
    char* words[BUFFER_SIZE / 2];
    int n = 0;

    // Read data from stdin
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Copy word into array
        int len = strlen(buffer);
        char* word = malloc(len);
        strncpy(word, buffer, buffer[len - 1] == '\n' ? len - 1 : len);
        words[n++] = word;
    }

    // Sort words
    qsort(words, n, sizeof(const char*), alphabetical_compare);

    // Free memory
    for (int i = 0; i < n - 1; i++) {
        printf("%s\n", words[i]);
        free(words[i]);
    }

    printf("%s", words[n - 1]);
    free(words[n - 1]);

    return 0;
}