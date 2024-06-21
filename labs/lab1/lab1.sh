#! /usr/bin/env bash

### TODO:

# get filename to compile from command line (how do you read just the firsy argument?)
filename=$1

# compile lab1.c to lab1 (you did something similar in Lab 0)
gcc -Wall -o lab1 $filename

# get the arguments passed when running lab1.sh (*this is the hard part)
nums=${@:2}

# run the lab1 program with those arguments (*this is the other hard part)
./lab1 $nums

