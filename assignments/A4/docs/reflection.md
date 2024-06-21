# Successes

- Naturally, getting this program to run at all is an achievement. The codebase is intentionally buggy and does not do anything significant when run without any changes.
- I was able to enable threaded keyboard input, albeit with some caveats (see Obstacles). 
- After some prototyping, the inklings just would not move, and I couldn't figure out why. Eventually, after some trial and error, I realized I was using an unnecessary mutex lock in the body of the inkling `threadFunction()`. This lock was blocking anything from happening. This was a simple fix, but through the debugging process, I ended up learning a lot about how threads interact with locks and each other.
- Our initial plan of using an OOP-based approach to logging allowed us to abstract that logic away from the main body of our code, simplifying our overall code structure.
- Through a simple logical change, I was able to prevent extra cells from being drawn when multiple inklings occupy a single cell. In doing so, I realized that I did not actually need to maintain a `grid` variable as shown in the stencil code. I was able to remove a sizeable amount of unused code this way.

# Obstacles

- I was unable to create a 100% functioning user input handler. My code gets stuck on a `while` loop that waits for `std::cin` such that the program will only exit when the user inputs a character. I haven't found any way around this other than `abort()`ing the program when no live inklings remain, which works but dumps an unpleasant error message to the screen.

# Changes

- In the design document, we mention that we'll need to research how to manage log files with "indefinite lifetimes". The approach I decided to take instead was to simply open and close the log files for each log action. This may slightly impact performance, but it prevents issues related to file handles that are never closed.
- We initially thought that we were required to "paint" cells that inklings moved over via the use of a `paint()` function. This was sidelined after realizing that it wasn't actually mentioned in the assignment handout, but would make for a good improvement.

# Research process

I didn't spend much time researching implementation details after finishing the design document, which turned out to be a mistake.
As it were, I didn't understand multithreading nearly as well as I thought going into this assignment.
Most of my research took place after writing broken code that I thought would work.

One difficulty resulted from my research into possible ways of reading user input.
I found the `conio.h` library that seemed to be a great option for reading from `stdin` without blocking execution.
Unfortunately, it is only available on Windows.
I generally try to avoid low-level and OS-specific functions when writing C/C++ code anyway, so this was probably for the better.

# Time expenditure

- Design document: 1 hour
- Initial implementation (collaborative): 2 hours
- Initial implementation (independent): 2 hours
- Debugging, refactoring: 4-6 hours (this took a lot longer than I anticipated)
- Code documentation: 2 hours
- Program improvements: 2 hours
- Reflection: 1 hour

TOTAL: approx. 17 hours

# Partner review

Effort & participation rating: 5

[partner] and I collaborated on the foundation of the program, in particular the inkling and producer thread functions.
After that, we split off and finished the assignment mostly independently to avoid our submissions being too similar.

In hindsight, it would have been wise for us to write a bit more code together before completing the assignment on our own.
We both underestimated how much work was left to do after this point.

That said, I would have taken a lot longer on this assignment without [partner]'s help.
She was able to quickly find solutions to challenging parts of the program.
I think we work well together and am open to collaborating with her again in the future.