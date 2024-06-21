# A2 Design Doc

## Overview

Since this assignment is composed of several small programs, I believe it is an excellent candidate for test-driven development. Before writing any code, I will create a shell script for each program that compares its output to a variety of expected outputs. This will ensure that each program is completed correctly and that a bug in one program doesn't cause while testing another program.

## Programs

This section describes the general approach I will take for each program, as well as any supplementary data structures or functions, if applicable.

1. sentence
    - C: Copy `readfile` from lab2
    - bash: `cat`
2. makewords
    - C: Copy `linebreaker` from lab2, replacing `strncpy` with a simple for loop that throws away consecutive spaces
    - bash: Use `IFS` variable to split string on space, then echo each word
3. lowercase
    - C: Loop through strings and replace all uppercase chars with their lowercase variant
    - bash: Lowercase syntax (`,,`)
4. sort
    - C: Sort char** array using `qsort()` 
    - bash: Custom quicksort implementation
5. unique
    - C: Iterate over list and use a hash set to keep track of duplicates
    - bash: Iterate over list and check against a "duplicates" array that uses strings as indices
6. mismatch
    - C: Hash each word in the dictionary, then hash each word from stdin and compare to the dictionary hash
    - bash: `diff`

## Required Research

Some concepts I will need to research in order to complete this assignment include:

- How to implement quicksort (in bash)
  - I have never written a quicksort algorithm myself, so I will need to research how it works. I'll also need to look into how to implement the algorithm using Bash syntax, which I don't have a great handle on yet.
- How to implement a hash set (in C)
  - I plan to use an existing library for this. I could implement this data structure myself, but I feel that it would not be a good use of my time when many high-quality implementations are available.