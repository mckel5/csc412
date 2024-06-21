#! /usr/bin/env bash

g++ -Wall -Wextra -pedantic -std=c++20 -g -O3 example.cpp -o example
g++ -Wall -Wextra -pedantic -std=c++20 -g -O3 example_fork.cpp -o example_fork