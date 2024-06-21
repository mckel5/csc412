// how can we import our local fio C library?
#include <stdio.h>
#include <stdlib.h>
#include "../fio/fio.h"

int main() {
    FILE *file;
    char filename[] = "test.text"; 
    char buffer[1024]; // buffer to store data

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