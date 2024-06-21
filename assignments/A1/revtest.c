#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reverse.h"

static int tests_run = 0;
static int tests_correct = 0;
static int tests_incorrect = 0;

#define USAGE "We detected no arguments. Running test suite...\n"
#define ERRORMSG "ERROR: The 2nd argument must match the the number of elements\n Please try again.\n"
#define EXIT_FAILURE 1

/**
 * checks if the elements are properly reversed
 * and returns an error message if not
 */
void assert_equal(char** expected, char** actual, const char* message, int len) {
    tests_run++;
    int element_correct = 0;
    int element_incorrect = 0;

    for (int i = 0; i < len; i++) {
        if (!strcmp(expected[i], actual[i])) {
            element_correct++;
            if (element_correct == len) tests_correct++;
        } else {
            element_incorrect++;
            if (element_correct == len) tests_incorrect++;
            printf("\033[0;31m");  // set color to red
            printf(
                "[FAILED Test %d: %s]: At index: %d, expected string: %s, actual "
                "string: %s\n",
                tests_run, message, i, expected[i], actual[i]);
            printf("\033[0m");  // reset text to default color
        }
    }

    // If no elements are wrong, the test passed! Print a message that says this :)
    if (element_incorrect == 0) {
        printf("\033[0;32m");  // set color to green
        printf("[PASSED Test %d: %s]\n", tests_run, message);
        printf("\033[0m");  // reset text to default color
    }
}

/**
 * checks if two strings are equal and returns an error message if not
 */
void assert_string_equal(char* expected, char* actual, char* message) {
    tests_run++;

    if (!strcmp(expected, actual)) {
        // Strings are equal
        tests_correct++;
        printf("\033[0;32m");  // set color to green
        printf("[PASSED Test %d: %s]\n", tests_run, message);
        printf("\033[0m");  // reset text to default color
    } else {
        // Strings are not equal
        tests_incorrect++;
        printf("\033[0;31m");  // set color to red
        printf("[FAILED Test %d: %s]: Expected string: '%s', Actual string: '%s'\n",
               tests_run, message, expected, actual);
        printf("\033[0m");  // reset text to default color
    }
}

/**
 * runs tests for student implementation of reverse
 */
void test_array_reverse(void (*func)(char** arr, int size), int testnum) {
    printf("\n   Starting tests for reverse_arr%d\n", testnum);

    // test 1: test single element
    char* str1[] = {"hello"};
    func(str1, 1);  // Use the function pointer
    assert_equal(str1, str1, "one element test", 1);

    // test 2: test three elements (odd number of elements)
    char* str2[] = {"hello", "my", "name"};
    char* reverse2[] = {"name", "my", "hello"};
    func(str2, 3);  // Use the function pointer
    assert_equal(str2, reverse2, "three element test", 3);

    // test 3: test four elements (even number of elements)
    char* str3[] = {"hello", "my", "name", "is"};
    char* reverse3[] = {"is", "name", "my", "hello"};
    func(str3, 4);  // Use the function pointer
    assert_equal(str3, reverse3, "four element test", 4);

    // test 4: test five elements
    char* str4[] = {"i", "love", "systems", "csc", "412"};
    char* reverse4[] = {"412", "csc", "systems", "love", "i"};
    func(str4, 5);  // Use the function pointer
    assert_equal(str4, reverse4, "five element test", 5);

    // test 5: test 2 elements
    char* str5[] = {"csc", "412"};
    char* reverse5[] = {"412", "csc"};
    func(str5, 2);  // Use the function pointer
    assert_equal(str5, reverse5, "two element test", 2);
}

/**
 * runs tests for student implementation of reverse
 */
void test_char_reverse(void (*func)(char*, int), int testnum) {
    printf("\n   Starting tests for test_char_reverse%d\n", testnum);

    // test 1: palindrome
    char str1[] = "madam";  // array of char (i.e, a string we can modify), which is modifiable
    char* rev1 = "madam";
    func(str1, 5);  // should not change the string
    assert_string_equal(str1, rev1, "palindrome test");

    // test 2: test single string
    char str2[] = "hello";  // array of char (i.e, a string we can modify), which is modifiable
    char* rev2 = "olleh";
    func(str2, 5);
    assert_string_equal(str2, rev2, "test simple string");

    // test 3: special characters
    char str3[] = "hello, world!";
    char* rev3 = "!dlrow ,olleh";
    func(str3, 13);
    assert_string_equal(str3, rev3, "special characters test");

    // test 4: leading space characters
    char str4[] = "  leading space";
    char* rev4 = "ecaps gnidael  ";
    func(str4, 15);
    assert_string_equal(str4, rev4, "leading space characters test");

    // test 5: trailing space characters
    char str5[] = "trailing space  ";
    char* rev5 = "  ecapS gniliart";
    func(str5, 16);
    assert_string_equal(str5, rev5, "trailing space characters test");

    // test special: Unicode Characters
    char str6[] = "こんにちは";
    char* rev6 = "はちにんこ";
    func(str6, 6);
    assert_string_equal(str6, rev6, "Unicode Characters special test!");
}

/**
 * Print the number of tests passed
 */
void print_summary_stats() {
    printf("\n Your code ran %d tests. \n Your reverse functions passed %d/45 tests needed to get full credit.\n", tests_run, tests_correct);
}

/*
 * Runs tests in C for local development.
 */
int main(int argc, char** argv) {
    if (argc == 1) {
        // Run test suite

        printf("%s", USAGE);

        test_array_reverse(&reverse_arr1, 1);
        test_array_reverse(&reverse_arr2, 2);
        test_array_reverse(&reverse_arr3, 3);
        test_array_reverse(&reverse_arr4, 4);
        test_array_reverse(&reverse_arr5, 5);

        test_char_reverse(&reverse_str1, 1);
        test_char_reverse(&reverse_str2, 2);
        test_char_reverse(&reverse_str3, 3);
        test_char_reverse(&reverse_str4, 4);
        test_char_reverse(&reverse_str5, 5);

        print_summary_stats();

    } else {
        // Reverse array passed through command line

        // Get number of elements to be reversed
        int num_elements = atoi(argv[1]);

        // Handle improper usage
        if (num_elements != argc - 2) {
            fprintf(stderr, "%s", ERRORMSG);
            return EXIT_FAILURE;
        }

        char* arr[num_elements];

        // Copy from argv to local array
        for (int i = 0; i < num_elements; i++) {
            arr[i] = argv[i + 2];
        }

        reverse_arr1(arr, num_elements);

        // Print reversed array to stdout
        for (int i = 0; i < num_elements; i++) {
            printf("%s ", arr[i]);
        }
        printf("\n");
    }

    return 0;
}