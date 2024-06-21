#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"
#include "mbstrings.h"

/** Updates the game by a single step, and modifies the game information
 * accordingly. Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: width of the board.
 *  - height: height of the board.
 *  - snake_p: pointer to your snake struct (not used until part 2!)
 *  - input: the next input.
 *  - growing: 0 if the snake does not grow on eating, 1 if it does.
 */
void update(int* cells, size_t width, size_t height, snake_t* snake_p,
            enum input_key input, int growing) {
    // `update` should update the board, your snake's data, and global
    // variables representing game information to reflect new state. If in the
    // updated position, the snake runs into a wall or itself, it will not move
    // and global variable g_game_over will be 1. Otherwise, it will be moved
    // to the new position. If the snake eats food, the game score (`g_score`)
    // increases by 1. This function assumes that the board is surrounded by
    // walls, so it does not handle the case where a snake runs off the board.

    if (g_game_over) return;

    // Choose new direction
    // Keep previous direction if no key pressed
    enum SNAKE_DIRECTION old_direction = snake_p->direction;
    int input_received = 1;
    switch (input) {
        case INPUT_UP:
            snake_p->direction = FACING_UP;
            break;
        case INPUT_DOWN:
            snake_p->direction = FACING_DOWN;
            break;
        case INPUT_LEFT:
            snake_p->direction = FACING_LEFT;
            break;
        case INPUT_RIGHT:
            snake_p->direction = FACING_RIGHT;
            break;
        default:
            input_received = 0;
            break;
    }

    // If the snake tries to back into itself, revert its direction change
    if (length_list(snake_p->position) > 1 && opposite_directions(snake_p->direction, old_direction)) {
        snake_p->direction = old_direction;
    }

    // Calculate new position
    int new_position = -1;
    int current_position = *(int*)get_first(snake_p->position);

    switch (snake_p->direction) {
        case FACING_UP:
            new_position = current_position - width;
            break;
        case FACING_DOWN:
            new_position = current_position + width;
            break;
        case FACING_RIGHT:
            new_position = current_position + 1;
            break;
        case FACING_LEFT:
            new_position = current_position - 1;
            break;
        default:
            break;
    }

    // Check for invalid move
    // It's okay to move onto a snake space if that space is currently occupied by the end of the tail
    if (cells[new_position] & FLAG_WALL || (cells[new_position] & FLAG_SNAKE && new_position != *(int*)get_last(snake_p->position))) {
        g_game_over = 1;
        return;
    }

    // Check for food
    int got_food = 0;
    if (cells[new_position] & FLAG_FOOD) {
        g_score++;
        got_food = 1;
        place_food(cells, width, height);
    }

    // Copy head to new position, rest of body remains in place
    // Increases length by one segment
    insert_first(&snake_p->position, &new_position, sizeof(new_position));
    cells[new_position] = FLAG_SNAKE;

    // Shorten body to maintain current length, if appropriate
    // If the snake ate some food, this is skipped
    if (!got_food || !growing) {
        int tail_end_position = *(int*)remove_last(&snake_p->position);
        // Avoid graphical errors when moving onto a space that the tail previously occupied
        if (tail_end_position != new_position) {
            cells[tail_end_position] = FLAG_PLAIN_CELL;
        }
    }
}

/** Sets a random space on the given board to food.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - width: the width of the board
 *  - height: the height of the board
 */
void place_food(int* cells, size_t width, size_t height) {
    /* DO NOT MODIFY THIS FUNCTION */
    unsigned food_index = generate_index(width * height);
    if (*(cells + food_index) == FLAG_PLAIN_CELL) {
        *(cells + food_index) = FLAG_FOOD;
    } else {
        place_food(cells, width, height);
    }
    /* DO NOT MODIFY THIS FUNCTION */
}

/** Prompts the user for their name and saves it in the given buffer.
 * Arguments:
 *  - `write_into`: a pointer to the buffer to be written into.
 */
void read_name(char* write_into) {
    while (1) {
        printf("Name: ");
        fflush(stdout);
        ssize_t bytes_read = read(STDIN_FILENO, write_into, 1000);
        
        if (bytes_read > 1) {
            // Delete the newline (added when the user presses Enter) from the string
            write_into[bytes_read - 1] = '\0';
            break;
        }

        puts("Name Invalid: must be longer than 0 characters.");
    }
}

/** Cleans up on game over — should free any allocated memory so that the
 * LeakSanitizer doesn't complain.
 * Arguments:
 *  - cells: a pointer to the first integer in an array of integers representing
 *    each board cell.
 *  - snake_p: a pointer to your snake struct. (not needed until part 2)
 */
void teardown(int* cells, snake_t* snake_p) {
    // TODO: implement!
    free(cells);
}

int opposite_directions(enum SNAKE_DIRECTION a, enum SNAKE_DIRECTION b) {
    return (
        (a == FACING_UP && b == FACING_DOWN) ||
        (a == FACING_DOWN && b == FACING_UP) ||
        (a == FACING_LEFT && b == FACING_RIGHT) ||
        (a == FACING_RIGHT && b == FACING_LEFT));
}