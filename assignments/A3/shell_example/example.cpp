#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

int main() {
    const char* program_name = "/bin/echo";  // use const char* for string literals
    const char* arg1 = "hello";
    const char* arg2 = "worlds";
    char* newargv[] = { const_cast<char*>(program_name), const_cast<char*>(arg1), const_cast<char*>(arg2), NULL };

    // this will replace the process image with the image of executable: 
    // `/bin/echo`and arguments newargv
    execv("/bin/hello", newargv);
    
    // should only get past here if execv() returns an error
    perror("execv failed");
    exit(EXIT_FAILURE);
}