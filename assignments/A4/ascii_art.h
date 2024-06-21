//
//  ascii_art.h
//
//  Authors: Jean-Yves Hervé and Shaun Wallace
//

#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

//-----------------------------------------------------------------------------
//  Data types
//-----------------------------------------------------------------------------

// Travel direction data type
// Note that if you define a variable
// TravelDirection dir = whatever;
// you get the opposite directions from dir as (NUM_TRAVEL_DIRECTIONS - dir)
// you get left turn from dir as (dir + 1) % NUM_TRAVEL_DIRECTIONS
enum TravelDirection {
    NORTH = 0,
    WEST,
    SOUTH,
    EAST,
    NUM_TRAVEL_DIRECTIONS
};

// bc enums with unicode characters some compilers do not like
const std::string iconDirections[] = {
    "\u2191",  // north / up
    "\u2190",  // west / left
    "\u2193",  // south / down
    "\u2192",  // east / right
    " ",       // nothing
};

// ANSII text colors
enum class TextColor {
    DEFAULT = 39,
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37
};

enum InklingType {
    RED_TRAV = 0,
    GREEN_TRAV,
    BLUE_TRAV,
    NUM_TRAV_TYPES
};

class Logger {
   private:
    // Log directory
    const std::string outputDir = "logFolder/";
    // Log file path
    std::string filename;
    // Number to be included in filename and log files (e.g. inkling4)
    int id;

    /**
     * Get the current time in `HH:MM:SS.mmm` format.
     * 
     * @returns A string representing the current time.
     * @see https://stackoverflow.com/a/35157784
    */
    std::string getCurrentTime() {
        using namespace std::chrono;

        // get current time
        auto now = system_clock::now();

        // get number of milliseconds for the current second
        // (remainder after division into seconds)
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        // convert to std::time_t in order to convert to std::tm (broken time)
        auto timer = system_clock::to_time_t(now);

        // convert to broken time
        std::tm bt = *std::localtime(&timer);

        std::ostringstream oss;

        oss << std::put_time(&bt, "%H:%M:%S");  // HH:MM:SS
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

        return oss.str();
    }

   public:
    /**
     * Construct a Logger object.
     *
     * @param id The ID of the coupled inkling. Determines the filename (e.g. id == 4 -> inkling4.txt)
     */
    Logger(int id) {
        this->id = id;

        // Create output directory with 755 permissions
        if (!fs::exists(outputDir)) {
            fs::create_directory(outputDir);
            fs::permissions(
                outputDir,
                fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read | fs::perms::others_exec,
                fs::perm_options::replace);
        }

        // Path to the log file
        filename = outputDir + "inkling" + std::to_string(id) + ".txt";
    }

    /**
     * Log the inkling's initial position.
     *
     * @param type The inkling's type (color).
     * @param row The inkling's current row.
     * @param col The inkling's current column.
     */
    void logInitialPosition(InklingType type, int row, int col) {
        std::string colors[3] = {"red", "green", "blue"};
        std::string name = "inkling" + id;
        std::string color = colors[type];
        std::string row_ = "row" + row;
        std::string col_ = "col" + col;

        std::ofstream file;
        // Open in write mode
        // This creates the file if it doesn't exist
        file.open(filename);
        file << getCurrentTime() << ","
             << "inkling" << id << ","
             << color << ","
             << "row" << row << ","
             << "col" << col << "\n";

        // Set 755 file permissions -- only needs to happen at file creation
        fs::permissions(
            filename,
            fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec | fs::perms::others_read | fs::perms::others_exec,
            fs::perm_options::replace);
    }

    /**
     * Log the inkling's updated position.
     *
     * @param dir The direction in which the inkling is facing.
     * @param row The inkling's current row.
     * @param col The inkling's current column.
     */
    void logNewPosition(TravelDirection dir, int row, int col) {
        std::string directions[4] = {"north", "west", "south", "east"};
        std::string name = "inkling" + id;
        std::string direction = directions[dir];
        std::string row_ = "row" + row;
        std::string col_ = "col" + col;

        std::ofstream file;
        // Open file in append mode
        file.open(filename, std::ios::app);
        file << getCurrentTime() << ","
             << "inkling" << id << ","
             << direction << ","
             << "row" << row << ","
             << "col" << col << "\n";
    }

    /**
     * Log the inkling's termination.
     */
    void logTermination() {
        std::string name = "inkling" + id;

        std::ofstream file;
        // Open file in append mode
        file.open(filename, std::ios::app);
        file << getCurrentTime() << ","
             << "inkling" << id << ","
             << "terminated\n";
    }
};

// Example of Inkling thread info data type
struct InklingInfo {
    InklingType type;
    // location of the inkling
    int row;
    int col;
    // in which direction is the inkling going?
    TravelDirection dir;
    // initialized to true, set to false if terminates
    bool isLive;
    Logger logger;
};

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------

void myEventLoop();
void enableRawMode();
void disableRawMode();
void keyListener();
void myKeyboard(unsigned char c);
void initializeFrontEnd(int argc, char** argv, void (*gridCB)(void), void (*stateCB)(void));
void drawGridAndInklingsASCII(int numRows, int numCols, std::vector<InklingInfo>& inklingList);
void drawState(int numLiveThreads, int redLevel, int greenLevel, int blueLevel);
void clearTerminal();
void cleanupAndQuit(const std::string& msg);
void slowdownProducers(void);
void speedupProducers(void);

#endif  // ASCII_ART_H
