## Change 1: Keyboard input


### Proposed Change Summary

While the program is running, the user can input commands that affect the outcome of the program.
These include refilling ink tanks and adjusting the speed of the program.
This is accomplished via a dedicated key listener thread.

### Describe the Problem

By default, the key handler code crashes the program immediately upon execution.
For the program to run properly, it must be disabled.

### Describe the Solution

The problem seemed to lie in the code that spawned the key listener thread.
This was called during each iteration of the main event loop, creating many more threads than necessary.
By limiting the key listener to one thread, keyboard input could be safely enabled.

### Describe the Implemented Solution

Instead of giving `myEventLoop()` the responsibility of spawning the key listener, I moved this code into `main()`.
This ensured that only one listener thread existed at a time.
I also altered `keyListener()` to include a loop that would continuously check for key presses.

### Describe how you Tested your Implemented Solution

While the program was running, I pressed each of the keys listed in `myKeyboard()` and observed the effects they had on the inklings and producers.
While the program no longer crashes, this change introduced the unfortunate side effect of requiring an additional keypress to exit the program, as `std::cin` blocks execution even after calling `exit()`.



## Change 2: Reduce complexity of main event loop


### Proposed Change Summary

`myEventLoop()` drives the execution of the program.
It is a recursive function that implements some thread-safety measures.
However, it can work just as well using an iterative approach.

### Describe the Problem

Initially, `myEventLoop()` is a recursive function with a max depth of 5 calls.
This depth is represented by an atomic integer and is updated by means of a mutex lock.
This is an extremely complex implementation of a conceptually simple function; all it needs to do is call `updateTerminal()` on a timer.
It does not benefit from recursion since each call is exactly the same and there is a miniscule limit on the recursive depth.
It also includes some keypress handling code, which is made redundant by the key listener thread.

### Describe the Solution

Everything that the event loop needs to do can be accomplished with a loop.
This will eliminate all thread-related code and vastly simplify the debugging experience.

### Describe the Implemented Solution

I rewrote `myEventLoop()` using a `while (true)` loop, then deleted all the key handler code.
This reduced the function's size from ~50 lines of code to fewer than 5.

### Describe how you Tested your Implemented Solution

I ran the program a few times to see if I could notice any change in functionality.
Not only did the program not run any worse, I also found that the program actually stopped crashing when dealing with large numbers of inklings, which was a problem that had stumped me previously.