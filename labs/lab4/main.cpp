#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

std::string directoryName = "logFolder";

void threadFunc(int num) {
    // calculate the factorial of num
    int factorial = 1;
    for (int i = 1; i <= num; i++) {
        factorial *= i;
    }

    // create a file with the proper
    // file needs permissions 0755 HINT: C++ chmod function
    std::string path(directoryName + "/thread" + std::to_string(num) + ".txt");
    std::ofstream file(path);
    chmod(path.c_str(), 0755);

    // write the information to the file and then close the file
    file << "This thread's value is " << num << ".\n";
    file << "The factorial of " << num << " is " << factorial << ".";
}

int genRandNumber(int min, int max) {
    // use std::random_device to obtain a seed
    std::random_device device;

    // use the random device to seed a random number engine:
    std::default_random_engine engine(device());

    // define the range [min, max]:
    std::uniform_int_distribution<std::default_random_engine::result_type> distrubution(min, max);

    // generate and return the random number
    return distrubution(engine);
}

// we are not using  char* argv[], to avoid warnings we use char**
int main(int argc, char **) {
    // generate random number using genRandNumber
    int num_threads = genRandNumber(1, 10);

    // check for arguments
    // if there are arguments print to stdout "You silly rabbit, this program accepts no arguments."
    if (argc > 1) {
        std::cout << "You silly rabbit, this program accepts no arguments; running with 3 threads.";
    }

    // create the directory (0755 are the permissions) - HINT: C++ function mkdir
    mkdir(directoryName.c_str(), 0755);

    // create the proper amount of threads
    // pass all required information to the threads
    // here is some free code, enjoy
    std::vector<std::thread> threadList;
    for (int i = 1; i <= num_threads; i++) {
        threadList.emplace_back(std::thread(threadFunc, i));
    }

    // wait for all threads to complete
    for (auto &thread : threadList) {
        thread.join();
    }

    return 0;
}
