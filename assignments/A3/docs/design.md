# Prototyping (coreutils)

I am relatively comfortable with stream I/O in C++ as well as the coreutils involved in this assignment, so I elected to start implementing the copycat versions straight away.
For the most part, I ran into few issues and was able to allow each program to share a similar control flow.
The one sticking point was the `tail` program, as my initial plan of manually seeking through `std::cin` was not possible.
Other than this hurdle, I was able to implement the rest of the utilities without a hitch.

For any program that could take input from either a file or stdin (except `tail`), I was able to abstract the logic into functions that accepted a generic `istream` as an argument.
This cut down on code duplication and improved readability.

For `wc` in particular, I examined the GNU C source code to understand how that version defined a "word."

# Planning (shell)

It seems like implementing the shell will be a bit more complicated than the utilities, so I will plan my implementation out first.
My plan is to address each TODO comment in order, which should yield a complete `do_fork` function.

Some things I will need to research before starting to program include:

- How to check whether a fork was successful
- How to run another program using `execv`
- What `waitpid` needs to run besides the child PID
- What `dup2` is and how it works

I should be able to find information on these points with some basic online searches.