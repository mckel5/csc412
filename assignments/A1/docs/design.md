# Part 1

## Approach

I have a good understanding of strings and pointers in C, so my implementation plan consists of:

1. Implement `swap_chr` and `reverse_str1`, then test to ensure everything is working
2. Implement `swap` and `reverse_arr1`, then test
3. Implement remaining reverse functions
4. Implement custom args in `revtest.c`
5. Complete `build.sh` to make compiling and testing easier

## Functions & data structures

For both the string and array scenarios, I plan to implement a few basic "for" loop algorithms. I also would like to include a recursive function as well as one that makes use of a stack (really just an array with some special helper functions). The other functions will not need any data structures besides the input array.

## Testing

After completing each reverse function, I will run the test suite to ensure everything works as expected and that I didn't accidentally break any other functions.

# Part 2

## Research

Some things I will need to research more before starting on the implementation are:

- How strings behave when the `\0` is moved around
- How arrays are allocated, especially when the size is unknown at compile time