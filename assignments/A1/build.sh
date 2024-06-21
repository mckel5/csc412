#! /usr/bin/env bash

# build the test program revtest
build() {
    gcc -Wall -Werror revtest.c reverse.c -o revtest
}

# call the build function 
build

# run the revtest program with all arguments passed
./revtest $@
