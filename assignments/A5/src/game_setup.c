#include "game_setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some handy dandy macros for decompression
#define E_CAP_HEX 0x45
#define E_LOW_HEX 0x65
#define S_CAP_HEX 0x53
#define S_LOW_HEX 0x73
#define W_CAP_HEX 0x57
#define W_LOW_HEX 0x77
#define DIGIT_START 0x30
#define DIGIT_END 0x39

/** Initializes the board with walls around the edge of the board.
 *
 * Modifies values pointed to by cells_p, width_p, and height_p and initializes
 * cells array to reflect this default board.
 *
 * Returns INIT_SUCCESS to indicate that it was successful.
 *
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 */
enum board_init_status initialize_default_board(int** cells_p, size_t* width_p,
                                                size_t* height_p) {
    *width_p = 20;
    *height_p = 10;
    int* cells = malloc(20 * 10 * sizeof(int));
    *cells_p = cells;
    for (int i = 0; i < 20 * 10; i++) {
        cells[i] = FLAG_PLAIN_CELL;
    }

    // Set edge cells!
    // Top and bottom edges:
    for (int i = 0; i < 20; ++i) {
        cells[i] = FLAG_WALL;
        cells[i + (20 * (10 - 1))] = FLAG_WALL;
    }
    // Left and right edges:
    for (int i = 0; i < 10; ++i) {
        cells[i * 20] = FLAG_WALL;
        cells[i * 20 + 20 - 1] = FLAG_WALL;
    }

    // Add snake
    cells[20 * 2 + 2] = FLAG_SNAKE;

    return INIT_SUCCESS;
}

/** Initialize variables relevant to the game board.
 * Arguments:
 *  - cells_p: a pointer to a memory location where a pointer to the first
 *             element in a newly initialized array of cells should be stored.
 *  - width_p: a pointer to a memory location where the newly initialized
 *             width should be stored.
 *  - height_p: a pointer to a memory location where the newly initialized
 *              height should be stored.
 *  - snake_p: a pointer to your snake struct (not used until part 2!)
 *  - board_rep: a string representing the initial board. May be NULL for
 * default board.
 */
enum board_init_status initialize_game(int** cells_p, size_t* width_p,
                                       size_t* height_p, snake_t* snake_p,
                                       char* board_rep) {
    // Ensure that the linked list is NULL before initialization
    // Undefined behavior will occur otherwise!
    snake_p->position = NULL;

    int board_init_status;
    if (board_rep == NULL) {
        board_init_status = initialize_default_board(cells_p, width_p, height_p);
        int position = 20 * 2 + 2;
        insert_first(&snake_p->position, &position, sizeof(position));
    } else {
        board_init_status = decompress_board_str(cells_p, width_p, height_p, snake_p, board_rep);
    }

    if (board_init_status != INIT_SUCCESS) return board_init_status;

    snake_p->direction = FACING_RIGHT;

    place_food(*cells_p, *width_p, *height_p);

    g_game_over = 0;
    g_score = 0;

    return board_init_status;
}

/**
 * Calculate the appropriate board index based on the given row, column, and board width.
 */
int board_position(int row, int col, int width) {
    return row * width + col;
};

/**
 * Fill a span of cells with the specified type (wall, empty, or snake).
 */
void fill_cell_block(char cell_type, int start_position, int block_length, int** cells_p) {
    int cell_flag;

    switch (cell_type) {
        case 'W':
            cell_flag = FLAG_WALL;
            break;
        case 'E':
            cell_flag = FLAG_PLAIN_CELL;
            break;
        case 'S':
            cell_flag = FLAG_SNAKE;
            break;
        default:
            break;
    }

    for (int i = 0; i < block_length; i++) {
        (*cells_p)[start_position + i] = cell_flag;
    }
}

/** Takes in a string `compressed` and initializes values pointed to by
 * cells_p, width_p, and height_p accordingly. Arguments:
 *      - cells_p: a pointer to the pointer representing the cells array
 *                 that we would like to initialize.
 *      - width_p: a pointer to the width variable we'd like to initialize.
 *      - height_p: a pointer to the height variable we'd like to initialize.
 *      - snake_p: a pointer to your snake struct (not used until part 2!)
 *      - compressed: a string that contains the representation of the board.
 * Note: We assume that the string will be of the following form:
 * B24x80|E5W2E73|E5W2S1E72... To read it, we scan the string row-by-row
 * (delineated by the `|` character), and read out a letter (E, S or W) a number
 * of times dictated by the number that follows the letter.
 */
enum board_init_status decompress_board_str(int** cells_p, size_t* width_p,
                                            size_t* height_p, snake_t* snake_p,
                                            char* compressed) {
    if (compressed[0] != 'B') return INIT_ERR_BAD_CHAR;
    // Trim B from string
    compressed = strtok(compressed, "B");

    int str_height = atoi(strtok(compressed, "x"));
    if (str_height <= 0) return INIT_ERR_BAD_CHAR;

    int str_width = atoi(strtok(NULL, "|"));
    if (str_width <= 0) return INIT_ERR_BAD_CHAR;

    *width_p = str_width;
    *height_p = str_height;
    *cells_p = malloc(*width_p * *height_p * sizeof(int));

    size_t cells_filled = 0;
    size_t snakes_seen = 0;
    size_t position = 0;

    char* token = strtok(NULL, "|");
    while (token != NULL) {
        char cell_type = ' ';
        int block_length = 0;

        int i;
        for (i = 0; token[i] != '\0'; i++) {
            // Character is a cell type
            // This means this is either the first cell block OR we've just finished parsing a cell block
            if (token[i] == 'W' || token[i] == 'E' || token[i] == 'S') {
                // If cell_type is not set, this must be the first block in the row
                if (cell_type == ' ') {
                    cell_type = token[i];
                    continue;
                }

                // If cell_type *is* set, we have already parsed a cell type and a length, so let's fill those cells
                fill_cell_block(cell_type, position, block_length, cells_p);
                cells_filled += block_length;
                if (cell_type == 'S') {
                    // snake_p->position = position;
                    insert_first(&snake_p->position, &position, sizeof(position));
                    snakes_seen += block_length;
                }
                position += block_length;

                // Reset for next cell block
                cell_type = token[i];
                block_length = 0;
                continue;
            }

            // Character is a number
            // Update the span length accordingly
            if (DIGIT_START <= token[i] && token[i] <= DIGIT_END) {
                block_length = block_length * 10 + (token[i] - DIGIT_START);
                continue;
            }

            // Character is neither a cell type or a number
            return INIT_ERR_BAD_CHAR;
        }

        // End of token reached
        // Fill the last cells of this row

        // Prevent out-of-bounds access before things get messy
        if (position + block_length > *width_p * *height_p) return INIT_ERR_INCORRECT_DIMENSIONS;

        fill_cell_block(cell_type, position, block_length, cells_p);
        cells_filled += block_length;
        if (cell_type == 'S') {
            insert_first(&snake_p->position, &position, sizeof(position));
            snakes_seen += block_length;
        }
        position += block_length;

        // Move to start of next row data
        token = strtok(NULL, "|");
    }

    if (cells_filled != *height_p * *width_p) return INIT_ERR_INCORRECT_DIMENSIONS;
    if (snakes_seen != 1) return INIT_ERR_WRONG_SNAKE_NUM;

    return INIT_SUCCESS;
}
