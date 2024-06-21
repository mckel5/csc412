#include <iostream>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    FILE* file = fopen("dup_test.txt", "w");
    dup2(fileno(file), STDOUT_FILENO);
    std::cout << "Hello world!" << std::endl;
    char* args[] = {"ls", "-la", NULL};
    execvp("ls", args);

    return 0;
}
