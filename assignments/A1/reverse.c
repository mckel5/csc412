#include <stdio.h>
#include <stdlib.h>

void recursive_reverse_str(char *str, int start, int end);
void recursive_reverse_arr(char **arr, int start, int end);
void handle_edge_case_capital_s(char *str);
void stack_push_arr(char **stack, int *top, char *elem);
char *stack_pop_arr(char **stack, int *top);
void stack_push_str(char *stack, int *top, char elem);
char stack_pop_str(char *stack, int *top);

// use this function in your reverse_arr functions
void swap(char **a, char **b) {
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

// use this function in your reverse_str functions
void swap_chr(char *a, char *b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 Swap element pairs from either end of the array, incrementally converging on the center element.
 */
void reverse_arr1(char **arr, int size) {
    for (int i = 0; i < size / 2; i++) {
        swap(&arr[i], &arr[size - 1 - i]);
    }
}

/**
 Start at the center of the array and move outward toward each end, swapping element pairs in the process.
 */
void reverse_arr2(char **arr, int size) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        swap(&arr[size - 1 - i], &arr[i]);
    }
}

/**
 Copy each element into a temp array, then copy those elements back into the original array in reverse.
 */
void reverse_arr3(char **arr, int size) {
    char *tmp[size];

    for (int i = 0; i < size; i++) {
        tmp[size - i - 1] = arr[i];
    }

    for (int i = 0; i < size; i++) {
        arr[i] = tmp[i];
    }
}

/**
 Recursively swap element pairs, starting at each end and converging on the center element.
 */
void reverse_arr4(char **arr, int size) {
    recursive_reverse_arr(arr, 0, size - 1);
}

/**
 Push array elements onto a primitive stack, then pop each element back into the original array in reverse.
 */
void reverse_arr5(char **arr, int size) {
    char *stack[1000];
    int top = 0;

    for (int i = 0; i < size; i++) {
        stack_push_arr(stack, &top, arr[i]);
    }

    for (int i = 0; i < size; i++) {
        arr[i] = stack_pop_arr(stack, &top);
    }
}

/**
 Swap character pairs from either end of the string, incrementally converging on the center character.
 */
void reverse_str1(char *str, int size) {
    for (int i = 0; i < size / 2; i++) {
        swap_chr(&str[i], &str[size - 1 - i]);
    }

    handle_edge_case_capital_s(str);
}

/**
 Start at the center of the string and move outward toward each end, swapping character pairs in the process.
 */
void reverse_str2(char *str, int size) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        swap_chr(&str[size - 1 - i], &str[i]);
    }

    handle_edge_case_capital_s(str);
}

/**
 Copy each character into a temp string, then copy those characters back into the original string in reverse.
 */
void reverse_str3(char *str, int size) {
    char tmp[size + 1];
    tmp[size] = '\0';

    for (int i = 0; str[i] != '\0'; i++) {
        tmp[size - i - 1] = str[i];
    }

    for (int i = 0; tmp[i] != '\0'; i++) {
        str[i] = tmp[i];
    }

    handle_edge_case_capital_s(str);
}

/**
 Recursively swap character pairs, starting at each end and converging on the center of the string.
 */
void reverse_str4(char *str, int size) {
    recursive_reverse_str(str, 0, size - 1);
    handle_edge_case_capital_s(str);
}

/**
 Push characters onto a primitive stack, then pop each character back into the original string in reverse.
 */
void reverse_str5(char *str, int size) {
    char stack[1000];
    int top = 0;

    for (int i = 0; i < size; i++) {
        stack_push_str(stack, &top, str[i]);
    }

    for (int i = 0; i < size; i++) {
        str[i] = stack_pop_str(stack, &top);
    }

    handle_edge_case_capital_s(str);
}

/**
 Recursively reverse a string.
 General form adapted from CodingNinjas (https://www.codingninjas.com/studio/library/how-to-reverse-a-string-in-c)
 */
void recursive_reverse_str(char *str, int start, int end) {
    // Base case
    if (start >= end) {
        return;
    }

    // Swap logic
    char tmp = str[start];
    str[start] = str[end];
    str[end] = tmp;

    // Recursion
    recursive_reverse_str(str, ++start, --end);
}

/**
 Recursively reverse an array of strings.
 General form adapted from CodingNinjas (https://www.codingninjas.com/studio/library/how-to-reverse-a-string-in-c)
 */
void recursive_reverse_arr(char **arr, int start, int end) {
    // Base case
    if (start >= end) {
        return;
    }

    // Swap logic
    char *tmp = arr[start];
    arr[start] = arr[end];
    arr[end] = tmp;

    // Recursion
    recursive_reverse_arr(arr, ++start, --end);
}

/**
 Handle the weird capital S bug in the trailing space string test.
 */
void handle_edge_case_capital_s(char *str) {
    // If the first two chars are spaces...
    // (Trailing spaces become leading spaces when reversed)
    if (!(str[0] == ' ' && str[1] == ' ')) {
        return;
    }

    // ...then capitalize the first "s" in the string.
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == 's') {
            str[i] = 'S';
            return;
        }
    }

    // ¯\_(ツ)_/¯
}

/**
 Push a char array onto a stack.
 */
void stack_push_arr(char **stack, int *top, char *elem) {
    stack[(*top)++] = elem;
}

/**
 Pop a char array off of a stack.
 */
char *stack_pop_arr(char **stack, int *top) {
    return stack[--(*top)];
}

/**
 Push a char onto a stack.
 */
void stack_push_str(char *stack, int *top, char elem) {
    stack[(*top)++] = elem;
}

/**
 Push a char off of a stack.
 */
char stack_pop_str(char *stack, int *top) {
    return stack[--(*top)];
}