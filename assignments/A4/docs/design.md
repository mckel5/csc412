# A4: Design Document

Marceline Kelly, [partner]

# Plan of Attack

## Familiarization

For this part, we will be taking our time to run the code in the program to learn the ASCII art and how it works. No development will be made in this phase.

## Create Inklings

This part will be focused primarily on creating inklings as well as multiple threads.
In order to complete this part, we will need an understanding of what inklings and threads are, as well as how to build and manipulate them.
Due to the implementation with the ASCII art, we will need to make multiple functions to track and use the inklings:

`genInk()`: generate an inkling at a random unique position
`locate()`: takes in an inkling and returns its position
`genTrav()`: generate a random amount of valid spaces to travel for an inkling
`refill()`: refills the tank of an inkling
`level()`: returns the ink levels of an inkling
`move()`: moves an inkling to a desired location
`paint()`: paints a tile a certain color

## Create Log files

To enable logging in our program, we plan to create a Logger class of which each Inkling will contain an instance (hooray for composition). Logger will handle file I/O and formatting to reduce clutter in the Inkling code. Inkling will call Logger methods whenever it performs an action, i.e. moving or terminating. The class might look something like this:

```cpp
class Logger {
    file log_file;
    int inkling_id;
    void log_initialization(InklingType color, int row, int col); 
    void log_position(TravelDirection direction, int row, int col);
    void log_termination();
}
```

We will also create a log compilation program that will combine each Inkling’s log file into a single, repeatable list of actions.

## Make Improvements

We will expand on this section in our improvements document, but our initial observations of opportunities for enhancements include:

- Include keyboard controls in CLI
- Clear up any bugs that might arise within the stencil code
- In the ASCII art, create a labeled X and Y label for easy counting
- Add a fourth Inkling type

Once implemented, we will be following the guidelines written for the improvements.md document.

# Potential Obstacles

The largest obstacle that might arise may come from the storing of multiple inklings as threads, as threads are a relatively new topic for us. Naturally, we'll likely run into program instability and data corruption at first due to improper thread management. Apart from this, we predict that we'll face the following challenges:

- Moving an inkling across the ASCII art
  - Research: how to effectively perform bounds checking with two-dimensional arrays
- Handling cells with multiple inklings
  - Research: how to manage shared resources such that the program doesn’t crash when multiple inklings try to claim the same cell
- Creating the correct log files
  - Research: how to properly open and close file handles that have indefinite lifetimes
- Terminating an inkling as it reaches a corner of the grid
  - Research: how to properly terminate a thread while allowing others to continue running


