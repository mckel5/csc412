Overall, I found this assignment to be straightforward to complete. I was able to prototype a considerable amount of code without issue and the research needed to implement the remaining parts was 

# Successes

## Code reuse

Since all the coreutil programs involve reading from and/or writing to text streams, I was able to reuse a good amount of code between them.
This cut down on development time and reduced the amount of debugging I had to do.

# Obstacles

## `stdin` seeking

In my design document, I mentioned that I had some difficulties reading from stdin in `tail`.
I planned to seek to the end of the stream and move backwards, printing each character one by one, but stdin can't be manipulated in this way.
After some deliberation, I decided to store each line read from stdin in a queue, whose oldest member would automatically be popped off after 10 lines.
This is likely a bit slower than the file-cursor approach, but it still does not require the entire input to be loaded into memory at once.

## Formatting specifications

It took a few tries for me to match my coreutil output formatting with that of the Autograder.
However, the detailed error messages provided for each test made it clear exactly what was expected, allowing me to quickly tweak my programs so that they passed each test.

## `execvp` arguments

While researching the `execvp` function, I had some trouble understanding how to convert the result of `convertArgs()` (char* vector) to a format that `execvp` would be able to use (char* array).
After a bit of searching, I found out that vectors can be passed to functions expecting a C-style array by using a reference to the first element, since vectors are really just C-style arrays with some nice helper methods built-in.

# Time usage

## coreutils

- Prototyping: 2 hours
- Revised implementations: 2 hours

## 412shell

- Function research (`fork`, `execvp`, `waitpid`, etc.): 30 minutes
- Initial implementation: 1 hour
- Debugging: 1 hour