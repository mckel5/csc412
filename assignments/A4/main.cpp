//
//  main.cpp
//  inklings
//
//  Authors: Jean-Yves Hervé, Shaun Wallace, and Luis Hernandez
//

/*--------------------------------------------------------------------------+
|	A graphic front end for a grid+state simulation.						|
|																			|
|	This application simply creates a colored grid and displays             |
|   some state information in the terminal using ASCII art.			        |
|	Only mess with this after everything else works and making a backup		|
|	copy of your project.                                                   |
|																			|
|	Current Keyboard Events                                     			|
|		- 'ESC' --> exit the application									|
|		- 'r' --> add red ink												|
|		- 'g' --> add green ink												|
|		- 'b' --> add blue ink												|
|		- '>' --> speed up ink producers                                    |
|		- '<' --> slow down ink producers									|
|		- '+' --> speed up inkling movement									|
|		- '-' --> slow down inkling movement								|
+--------------------------------------------------------------------------*/

#include <unistd.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

#include "ascii_art.h"

//==================================================================================
//	Function prototypes
//==================================================================================
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void threadFunction(InklingInfo* inkling);
void getNewDirection(InklingInfo* inkling);
bool checkIfInCorner(InklingInfo* inkling);
void redColorThreadFunc();
void greenColorThreadFunc();
void blueColorThreadFunc();
bool checkEnoughInk(InklingInfo* inkling, int moveAmount);

//==================================================================================
//	Application-level global variables
//==================================================================================

//	The state grid dimensions
int NUM_ROWS, NUM_COLS;

//	the number of live threads (that haven't terminated yet)
int MAX_NUM_TRAVELER_THREADS;
int numLiveThreads = 0;

// vector to store each struct
std::vector<InklingInfo> info;
bool DRAW_COLORED_TRAVELER_HEADS = true;

//	the ink levels
int MAX_LEVEL = 50;
int MAX_ADD_INK = 10;
int redLevel = 20, greenLevel = 10, blueLevel = 40;

// create locks for color levels
std::mutex redLock;
std::mutex blueLock;
std::mutex greenLock;
std::mutex blueCellLock;
std::mutex redCellLock;
std::mutex greenCellLock;

// ink producer sleep time (in microseconds)
// [min sleep time is arbitrary]
const int MIN_SLEEP_TIME = 30000;
int producerSleepTime = 100000;

// inkling sleep time (in microseconds)
int inklingSleepTime = 1000000;

// Random number engine
std::random_device dev;
std::default_random_engine rngEngine(dev());

//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your help to ensure
//	that access to critical data and the ASCII art are properly synchronized
//==================================================================================

/**
 * Render the grid on stdout using ASCII art.
 */
void displayGridPane(void) {
    drawGridAndInklingsASCII(NUM_ROWS, NUM_COLS, info);
}

/**
 * Display ink levels and live thread information on stdout.
 */
void displayStatePane(void) {
    drawState(numLiveThreads, redLevel, greenLevel, blueLevel);
}

/**
 * Consume ink from the red ink tank.
 * Called by inkling threads.
 *
 * @param theRed The amount of ink to consume.
 */
void acquireRedInk(int theRed) {
    redLock.lock();
    if (redLevel >= theRed) {
        redLevel -= theRed;
    }
    redLock.unlock();
}

/**
 * Consume ink from the green ink tank.
 * Called by inkling threads.
 *
 * @param theGreen The amount of ink to consume.
 */
void acquireGreenInk(int theGreen) {
    greenLock.lock();
    if (greenLevel >= theGreen) {
        greenLevel -= theGreen;
    }
    greenLock.unlock();
}

/**
 * Consume ink from the blue ink tank.
 * Called by inkling threads.
 *
 * @param theBlue The amount of ink to consume.
 */
void acquireBlueInk(int theBlue) {
    blueLock.lock();
    if (blueLevel >= theBlue) {
        blueLevel -= theBlue;
    }
    blueLock.unlock();
}

//------------------------------------------------------------------------
//	These are the functions that would be called by a producer thread in
//	order to refill the red/green/blue ink tanks.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------

/**
 * Refill the red ink tank.
 * Called by producer threads.
 * Does nothing if the caller tries to fill the tank with more ink than it can hold.
 *
 * @param theRed The amount of ink to fill.
 */
void refillRedInk(int theRed) {
    redLock.lock();
    if (redLevel + theRed <= MAX_LEVEL) {
        redLevel += theRed;
    }
    redLock.unlock();
}

/**
 * Refill the green ink tank.
 * Called by producer threads.
 * Does nothing if the caller tries to fill the tank with more ink than it can hold.
 *
 * @param theGreen The amount of ink to fill.
 */
void refillGreenInk(int theGreen) {
    greenLock.lock();
    if (greenLevel + theGreen <= MAX_LEVEL) {
        greenLevel += theGreen;
    }
    greenLock.unlock();
}

/**
 * Refill the blue ink tank.
 * Called by producer threads.
 * Does nothing if the caller tries to fill the tank with more ink than it can hold.
 *
 * @param theBlue The amount of ink to fill.
 */
void refillBlueInk(int theBlue) {
    blueLock.lock();
    if (blueLevel + theBlue <= MAX_LEVEL) {
        blueLevel += theBlue;
    }
    blueLock.unlock();
}

/**
 * Increase the speed of the producers by 20%.
 * Does nothing if the new speed would fall below `MIN_SLEEP_TIME`.
 */
void speedupProducers(void) {
    // decrease sleep time by 20%, but don't get too small
    int newSleepTime = (8 * producerSleepTime) / 10;

    if (newSleepTime > MIN_SLEEP_TIME) {
        producerSleepTime = newSleepTime;
    }
}

/**
 * Decrease the speed of the producers by 20%.
 * Does nothing if the new speed would fall below `MIN_SLEEP_TIME`.
 */
void slowdownProducers(void) {
    // increase sleep time by 20%
    producerSleepTime = (12 * producerSleepTime) / 10;
}

int main(int argc, char** argv) {
    // a try/catch block for debugging to catch weird errors in your code
    try {
        // check that arguments are valid, must be a 20x20 or greater and at least 8 threads/inklings
        if (argc == 4) {
            if (std::stoi(argv[1]) >= 20 && std::stoi(argv[2]) >= 20 && std::stoi(argv[3]) >= 8) {
                NUM_ROWS = std::stoi(argv[1]);
                NUM_COLS = std::stoi(argv[2]);
                MAX_NUM_TRAVELER_THREADS = std::stoi(argv[3]);
                numLiveThreads = std::stoi(argv[3]);
            }
        } else {
            std::cout << "No arguments provided, running with 8x8 grid and 4 threads.\n\tThis message will dissapear in 2 seconds... \n";
            sleep(2);  // so the user can read the message in std::cout one line up
            clearTerminal();
            // Default parameters
            NUM_ROWS = 8;
            NUM_COLS = 8;
            MAX_NUM_TRAVELER_THREADS = 4;
            numLiveThreads = 4;
        }

        initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);

        initializeApplication();

        // create producer threads that check the levels of each ink
        std::vector<std::thread> producers;
        producers.emplace_back(std::thread(redColorThreadFunc));
        producers.emplace_back(std::thread(greenColorThreadFunc));
        producers.emplace_back(std::thread(blueColorThreadFunc));

        // create threads for the inklings
        std::vector<std::thread> inklings;

        // Resorting to traditional for loop as the program does not compile otherwise
        for (int i = 0; i < info.size(); i++) {
            inklings.emplace_back(std::thread(threadFunction, &info[i]));
        }

        // now we enter the main event loop of the program
        myEventLoop();

        // ensure main does not return immediately, killing detached threads
        std::this_thread::sleep_for(std::chrono::seconds(30));

    } catch (const std::exception& e) {
        std::cerr << "ERROR :: Oh snap! unhandled exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :: Red handed! unknown exception caught" << std::endl;
    }

    return 0;
}

/**
 * Exit the program.
 *
 * @param msg A message to be printed to stdout when called
 */
void cleanupAndQuit(const std::string& msg) {
    std::cout << "Somebody called quits, goodbye sweet digital world, this was their message: \n"
              << msg;

    // For future consideration: joining all remaining threads here may prevent "ugly termination issues"

    // Hand keyboard control back to the user
    disableRawMode();

    std::cout << "\n\nEnter any character to exit...\n";

    exit(0);
}

/**
 * Initialize the application.
 * Creates all `InklingInfo` objects.
 */
void initializeApplication(void) {
    // Create RNG distributions
    std::uniform_int_distribution<std::default_random_engine::result_type> row_dist(0, NUM_ROWS - 1);
    std::uniform_int_distribution<std::default_random_engine::result_type> col_dist(0, NUM_COLS - 1);
    std::uniform_int_distribution<std::default_random_engine::result_type> type_dist(0, NUM_TRAV_TYPES - 1);
    std::uniform_int_distribution<std::default_random_engine::result_type> direction_dist(0, NUM_TRAVEL_DIRECTIONS - 1);

    // Determine whether a row/col candidate is a corner
    auto in_corner = [](int row, int col) {
        return (
            row == 0 && col == 0 || row == 0 && col == NUM_COLS - 1 || row == NUM_ROWS - 1 && col == 0 || row == NUM_ROWS - 1 && col == NUM_COLS - 1);
    };

    // Create a temporary grid to keep track of where existing inklings have been placed
    std::vector<std::vector<int>> grid(NUM_ROWS, std::vector<int>(NUM_COLS));

    // Initialize inklings
    for (int i = 0; i < MAX_NUM_TRAVELER_THREADS; i++) {
        // Choose coordinates, rerolling if grid already occupied or in corner
        int row, col;
        do {
            row = row_dist(rngEngine);
            col = col_dist(rngEngine);
        } while (grid[row][col] == 1 || in_corner(row, col));

        // Choose type, direction
        auto type = static_cast<InklingType>(type_dist(rngEngine));
        auto direction = static_cast<TravelDirection>(direction_dist(rngEngine));

        // Construct inkling
        info.push_back({type, row, col, direction, true, Logger(i + 1)});

        // Log inkling's initial position
        info[i].logger.logInitialPosition(type, row, col);

        // Mark cell to prevent future inklings from being placed here
        grid[row][col] = 1;
    }
}

/**
 * Executed by all inkling threads during their lifetime.
 * Responsible for moving the inklings and killing them when they reach a corner.
 *
 * @param inkling The inkling represented by the current thread.
 */
void threadFunction(InklingInfo* inkling) {
    std::uniform_int_distribution<std::default_random_engine::result_type> waitDist(MIN_SLEEP_TIME, inklingSleepTime);

    while (inkling->isLive) {
        // Wait for a random amount of time
        std::this_thread::sleep_for(std::chrono::microseconds(waitDist(rngEngine)));

        // Calculate the maximum distance the inkling can move
        int maxDist;
        switch (inkling->dir) {
            case NORTH:
                maxDist = inkling->row;
                break;
            case EAST:
                maxDist = NUM_COLS - inkling->col - 1;
                break;
            case SOUTH:
                maxDist = NUM_ROWS - inkling->row - 1;
                break;
            case WEST:
                maxDist = inkling->col;
                break;
        }

        std::uniform_int_distribution<std::default_random_engine::result_type> distance(1, maxDist);
        // Prevent undefined behavior when maxDist < 1
        int dist = maxDist <= 0 ? 0 : distance(rngEngine);

        // Calculate the actual distance to move based off the amount of ink available
        // Do not move if insufficient ink is available
        switch (inkling->type) {
            case RED_TRAV:
                if (!checkEnoughInk(inkling, dist)) {
                    acquireRedInk(redLevel);
                    dist = redLevel;
                } else {
                    acquireRedInk(dist);
                }
                break;

            case GREEN_TRAV:
                if (!checkEnoughInk(inkling, dist)) {
                    acquireGreenInk(greenLevel);
                    dist = greenLevel;
                } else {
                    acquireGreenInk(dist);
                }
                break;

            case BLUE_TRAV:
                if (!checkEnoughInk(inkling, dist)) {
                    acquireBlueInk(blueLevel);
                    dist = blueLevel;
                } else {
                    acquireBlueInk(dist);
                }
                break;
        }

        // Move the inkling by the calculated distance
        switch (inkling->dir) {
            case NORTH:
                inkling->row -= dist;
                break;
            case SOUTH:
                inkling->row += dist;
                break;
            case EAST:
                inkling->col += dist;
                break;
            case WEST:
                inkling->col -= dist;
                break;
        }

        // Log new position
        inkling->logger.logNewPosition(inkling->dir, inkling->row, inkling->col);

        // Terminate thread if inkling is in a corner
        if (checkIfInCorner(inkling)) {
            // Wait 1ms to prevent sorting issues caused by identical timestamps when combining log files
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            // Log inkling termination
            inkling->logger.logTermination();

            inkling->isLive = false;
            numLiveThreads--;

            // Terminate thread
            return;
        }

        // If not terminated, choose a new movement direction
        getNewDirection(inkling);
    }
}

/**
 * Choose a new direction for an inkling to move in.
 * Must be perpendicular to the inkling's current direction (e.g. if the inkling is facing north, it can move east or west).
 *
 * @param inkling The inkling represented by the current thread.
 */
void getNewDirection(InklingInfo* inkling) {
    // Since an inkling can only turn left or right, we only need a binary value
    std::uniform_int_distribution<std::default_random_engine::result_type> directionDist(0, 1);
    int randNum = directionDist(rngEngine);

    // Choose new direction using random number
    switch (inkling->dir) {
        case NORTH:
        case SOUTH:
            inkling->dir = randNum == 0 ? EAST : WEST;
            break;
        case EAST:
        case WEST:
            inkling->dir = randNum == 0 ? NORTH : SOUTH;
            break;
    }
}

/**
 * Determine whether an inkling is in a corner of the grid.
 *
 * @param inkling The inkling represented by the current thread.
 * @returns True if in a corner, false otherwise.
 */
bool checkIfInCorner(InklingInfo* inkling) {
    return (
        (inkling->col == 0 && inkling->row == 0) ||
        (inkling->col == 0 && inkling->row == NUM_ROWS - 1) ||
        (inkling->col == NUM_COLS - 1 && inkling->row == 0) ||
        (inkling->col == NUM_COLS - 1 && inkling->row == NUM_ROWS - 1));
}

/**
 * Determine whether there is enough ink available to move the desired distance.
 *
 * @param inkling The inkling represented by the current thread.
 * @param distance The distance the inkling wants to move.
 * @returns True if there is sufficient ink, false otherwise.
 */
bool checkEnoughInk(InklingInfo* inkling, int distance) {
    bool hasEnoughInk = false;

    switch (inkling->type) {
        case RED_TRAV:
            redLock.lock();
            hasEnoughInk = (redLevel >= distance);
            redLock.unlock();
        case GREEN_TRAV:
            greenLock.lock();
            hasEnoughInk = (greenLevel >= distance);
            greenLock.unlock();
        case BLUE_TRAV:
            blueLock.lock();
            hasEnoughInk = (greenLevel >= distance);
            blueLock.unlock();
    }

    return hasEnoughInk;
}

/**
 * Executed by the red ink producer thread.
 */
void redColorThreadFunc() {
    std::uniform_int_distribution<std::default_random_engine::result_type> waitDist(MIN_SLEEP_TIME, producerSleepTime);

    while (numLiveThreads > 0) {
        // Wait for a random amount of time
        std::this_thread::sleep_for(std::chrono::microseconds(waitDist(rngEngine)));
        // Refill the red ink if it is empty
        if (redLevel <= 0) refillRedInk(MAX_ADD_INK);
    }
}

/**
 * Executed by the green ink producer thread.
 */
void greenColorThreadFunc() {
    std::uniform_int_distribution<std::default_random_engine::result_type> waitDist(MIN_SLEEP_TIME, producerSleepTime);

    while (numLiveThreads > 0) {
        // Wait for a random amount of time
        std::this_thread::sleep_for(std::chrono::microseconds(waitDist(rngEngine)));
        // Refill the green ink if it is empty
        if (greenLevel <= 0) refillGreenInk(MAX_ADD_INK);
    }
}

/**
 * Executed by the blue ink producer thread.
 */
void blueColorThreadFunc() {
    std::uniform_int_distribution<std::default_random_engine::result_type> waitDist(MIN_SLEEP_TIME, producerSleepTime);

    while (numLiveThreads > 0) {
        // Wait for a random amount of time
        std::this_thread::sleep_for(std::chrono::microseconds(waitDist(rngEngine)));
        // Refill the blue ink if it is empty
        if (blueLevel <= 0) refillBlueInk(MAX_ADD_INK);
    }
}