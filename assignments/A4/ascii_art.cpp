//
//  ascii_art.cpp
//
//  Authors: Jean-Yves Hervé, Shaun Wallace, and Luis Hernandez
//

#include "ascii_art.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

//---------------------------------------------------------------------------
//	ink access functions.
//---------------------------------------------------------------------------

void acquireRedInk(int theRed);
void acquireGreenInk(int theGreen);
void acquireBlueInk(int theBlue);
void refillRedInk(int theRed);
void refillGreenInk(int theGreen);
void refillBlueInk(int theBlue);

//---------------------------------------------------------------------------
//  Private functions' prototypes
//---------------------------------------------------------------------------

void myKeyboard(unsigned char c);
void myEventLoop(int val);

//---------------------------------------------------------------------------
//  Interface constants
//---------------------------------------------------------------------------

extern bool DRAW_COLORED_TRAVELER_HEADS;
extern int inklingSleepTime;

//---------------------------------------------------------------------------
//  File-level global variables
//---------------------------------------------------------------------------

void (*gridDisplayFunc)(void);
void (*stateDisplayFunc)(void);

extern int MAX_LEVEL;
extern int MAX_ADD_INK;
extern int MAX_NUM_TRAVELER_THREADS;

// producer sleep times
extern int producerSleepTime;

// Ensures only one key listener is spawned
bool keyListenerInitialized = false;

//---------------------------------------------------------------------------
//	Util Terminal Print ASCII functions
//---------------------------------------------------------------------------

// set text color
void setTextColor(TextColor color) {
    std::cout << "\033[" << static_cast<int>(color) << "m";
}

// reset text color to default
void resetTextColor() {
    setTextColor(TextColor::DEFAULT);
}

// base function to be reused
void print() {
    resetTextColor();  // reset color at the end of printing
    std::cout << std::endl;
}

// variadic template function for the magic sauce
template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    if constexpr (std::is_same_v<T, TextColor>) {
        setTextColor(first);
    } else {
        std::cout << first;
    }
    print(rest...);
}

// base function to be reused
void printCell() {
    resetTextColor();  // reset color at the end of printing
}

// variadic template function for the magic sauce
template <typename T, typename... Args>
void printCell(const T& first, const Args&... rest) {
    if constexpr (std::is_same_v<T, TextColor>) {
        setTextColor(first);
    } else {
        std::cout << first;
    }
    printCell(rest...);
}

void clearTerminal() {
    std::cout << "\033[H\033[J";  // clear the terminal screen ;)
}

//---------------------------------------------------------------------------
//	Drawing functions
//---------------------------------------------------------------------------

void drawGridAndInklingsASCII(int numRows, int numCols, std::vector<InklingInfo>& inklingList) {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            bool inklingFound = false;
            // is there a inkling in this grid spot?
            for (auto& inkling : inklingList) {
                if (inkling.isLive) {
                    if (row == inkling.row && col == inkling.col) {
                        // BUG high grid count: extra vertical lines, but no extra horizontal

                        switch ((int)(inkling.type)) {
                            case RED_TRAV:
                                printCell(TextColor::RED, "[", iconDirections[inkling.dir], "]");
                                inklingFound = true;
                                break;
                            case GREEN_TRAV:
                                printCell(TextColor::GREEN, "[", iconDirections[inkling.dir], "]");
                                inklingFound = true;
                                break;
                            case BLUE_TRAV:
                                printCell(TextColor::BLUE, "[", iconDirections[inkling.dir], "]");
                                inklingFound = true;
                                break;
                        }

                        // Break out of the loop so that two inklings are not drawn on the same cell
                        break;
                    }
                }
            }
            if (!inklingFound) {
                printCell(TextColor::BLACK, "[ ]");
            }
        }
        std::cout << std::endl;  // create new row
    }
}

void drawState(int numLiveThreads, int redLevel, int greenLevel, int blueLevel) {
    // build, then display text info for the red, green, and blue tanks
    print(TextColor::BLACK, "Ink Tank Levels, the MAX is: ", MAX_LEVEL);
    print(TextColor::RED, "Red: ", redLevel, TextColor::GREEN, " Green: ", greenLevel, TextColor::BLUE, " Blue: ", blueLevel);

    // display info about number of live threads
    print("Live Threads: ", numLiveThreads);
    print();

    if (numLiveThreads == 0) {
        cleanupAndQuit("drawState callint it quits, no threads left...");
    }
}

/**
 * Update the terminal output.
 */
void updateTerminal(void) {
    try {
        clearTerminal();
        gridDisplayFunc();
        stateDisplayFunc();
    } catch (const std::exception& e) {
        std::cerr << "ERROR :/ updateTerminal :: caught exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :/ updateTerminal :: caught an unknown exception" << std::endl;
    }
}

//---------------------------------------------------------------------------
//	Keyboard functions
//---------------------------------------------------------------------------

/**
 * Enable raw mode, preventing user input from being written to the terminal.
 */
void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);  // disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * Disable raw mode, allowing user input to be written to the terminal.
 */
void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;  // re-enable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * Executed by the key listener thread.
 */
void keyListener() {
    char c;
    while (std::cin >> c) {
        myKeyboard(c);
    }
}

/**
 * Handles user keyboard input.
 *
 * @param c The key that the user pressed.
 */
void myKeyboard(unsigned char c) {
    switch (c) {
        // 'esc' to quit
        case 27:  // 27 is the ASCII value for ESC
            cleanupAndQuit("I see you pressed ESC, game over...");
            break;

        // slow down the production of ink for the producer threads
        case '<':
            slowdownProducers();
            break;

        // speed up the production of ink for the producer threads
        case '>':
            speedupProducers();
            break;

        // make inklings move faster
        case '+':
            if (inklingSleepTime - 100000 > 0) {
                inklingSleepTime -= 100000;
            }
            break;

        // make inklings move slower
        case '-':
            if (inklingSleepTime + 100000 < 2147483647) {
                inklingSleepTime += 100000;
            }
            break;

        // refill red ink
        case 'r':
            refillRedInk(MAX_ADD_INK);
            break;

        // refill green ink
        case 'g':
            refillGreenInk(MAX_ADD_INK);
            break;

        // refill blue ink
        case 'b':
            refillBlueInk(MAX_ADD_INK);
            break;
    }

    // regenerate the interface
    updateTerminal();
}

//---------------------------------------------------------------------------
//	Timer functions
//---------------------------------------------------------------------------

// custom timer function
void customTimerFunc(int milliseconds, std::function<void(int)> func, int val) {
    std::thread([=]() {
        try {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            func(val);
        } catch (const std::exception& e) {
            std::cerr << "ERROR darn :: customTimerFunc :: exception in thread: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "ERROR ah fudge :: customTimerFunc :: unknown exception in thread" << std::endl;
        }
    }).detach();
}

/**
 * Main event loop that drives the program.
 */
void myEventLoop() {
    // executes in the main thread
    while (true) {
        updateTerminal();

        // Start the key listener if it has not yet been created
        if (keyListenerInitialized == false) {
            keyListenerInitialized = true;
            enableRawMode();
            std::thread listenerThread(keyListener);
            listenerThread.detach();
        }

        sleep(1);
    }
}

/**
 * Initialize the terminal display.
 *
 * @param argc Number of arguments passed to `main()`.
 * @param argv Arguments passed to `main()`.
 * @param gridDisplayCB Callback function to update the grid.
 * @param stateDisplayCB Callback function to update the program state.
 */
void initializeFrontEnd(int argc, char** argv, void (*gridDisplayCB)(void), void (*stateDisplayCB)(void)) {
    gridDisplayFunc = gridDisplayCB;
    stateDisplayFunc = stateDisplayCB;
    updateTerminal();
}
