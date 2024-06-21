// how can we import our local fio C library?
#include <stdio.h>
#include <stdlib.h>

#include "../fio/fio.h"

#define BUFFER_SIZE (1024 * 1024)  // 1 MiB

int main() {
    FILE *file;
    char filename[] = "unix_sentence.text";
    char buffer[BUFFER_SIZE];  // buffer to store data

    // open the file using the local fio library (Hint: see line 1)
    file = openFile(filename);

    // Read and print the contents of the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // put file contents (buffer) into stdout (Hint: look at linebreaker)
        fputs(buffer, stdout);
    }

    // Close the file
    fclose(file);

    return 0;
}