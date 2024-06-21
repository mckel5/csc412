# A5: Design Doc

Marceline Kelly

## Describe in detail which parts of the assignments you will attempt in what order. This is your plan!

The writers of the handout have so graciously labeled each part of the assignment with a suggested order.
I see no reason to deviate from this approach, so I will aim for the following order:

1. Initialization - game state, board, and snake
2. Execution - making the game actually do something, adding fail conditions, handling user input
3. Support for custom boards using run-length-encoded strings
4. Adding a score element & allowing the snake to grow by using a linked list to keep track of each body segment
5. Displaying a game over screen with a custom name chosen by the user

## Describe at least two areas you think will be difficult or time consuming.

I think that incorporating a linked list into my game is going to be challenging.
Linked lists have always been a little confusing for me, especially in C, where you have to manually manage pointers and allocate memory on the fly.
I'm guessing that I'm going to run into some memory issues at first. 
Keeping the list functional and correct after each and every tick is going to be tough.

Reading custom boards supplied by the user seems like it could also come with a lot of pitfalls.
From reading over that section, it looks like there are a lot of different types of invalid data that I'll have to handle.
I'll also need to perform many conversions from characters to integers, enum values, etc. which in C can cause some problematic behavior if not done properly.

## Then, describe what you need to research to solve these problems.

First, I need to research what void pointers are in C.
I know that they can be used to represent any data type, but I'm not sure how to actually use them for this.
I want to avoid cluttering my code with all sorts of casts as much as possible.

Then, I'll need to look into the `strtok` function.
This seems like it will greatly simplify the process of reading user input.
I used it a bit in A2, but not in the same way that this program requires (calling the function repeatedly until the input is exhausted).
I'm going to have to carefully examine how this function operates to get the results I need and to minimize the problems caused by bad input.

## Provide a rough outline of the functions, data structures, and/or comments describing what you think you will need.

### Data structures

Global state variables
* `score`: int
* `game_over`: boolean

Snake
* `position`: linked list of board cells
  * Related functions include `insert()`, `get()`, and `remove()`
* `direction`: enum

Board
* `cells`: 2D array of integers that describe what each cell contains 
  * Wall, snake, food, or empty

### Functions

* `initialize_game()`: draw the board and place the snake along with some food
  * `decompress_board_str()`: read in a user-supplied board layout
* `update()`: the main loop of the program, drives the game's primary logic
* `end_game()`: cleans up when the user loses the game
* `read_name()`: allow the user to enter their name, to be displayed on the game over screen
  * Must support Unicode characters