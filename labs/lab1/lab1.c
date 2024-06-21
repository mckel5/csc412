#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    this is the main function. It takes in the command line arguments.
*/
int main(int argc, char *argv[]) {
    int num_count = argc - 1;
    int nums[num_count];
    
    for (int n = 1; n < num_count + 1; n++) {
        nums[n - 1] = atoi(argv[n]);
    }

    for (int i = 0; i < num_count; i++) {
        printf("%i\n", nums[i]);
    }

    return 0;
}