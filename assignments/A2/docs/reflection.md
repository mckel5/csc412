# Changes

* `unique`: My initial plan for this program was to use a hash set to keep track of all the words already seen, testing if a word had already been seen before printing. However, once I learned that the input was guaranteed to be sorted, I could simplify this program greatly. For both the C and Bash versions, I simply print each word if and only if the previous word is different.
* `mismatch` (bash): I probably could have figured out a way to use `diff`, but I believe its output would have required a lot of formatting to match the specification. Instead, I opted for a simple iterative solution, which takes up a relatively small number of lines.

# Challenges
By far, the most challenging aspect of this assignment was implementing a custom sorting algorithm in Bash. Bash is an interesting language because it can accomplish many of the same tasks as standard general-purpose languages, but its syntax does not make it easy. Returning a value from `partition()` was tricky, requiring the use of a global variable since Bash does not support traditional return values. The distinction between `[[ ]]` and `(( ))` in comparisons tripped me up a few times, as I found out when I realized that the program was comparing integers as strings and, naturally, getting the order wrong.

I'd go as far as to say that a majority of my time on this assignment was spent researching how to implement procedures I was already familiar with in Bash. That said, I feel like I have a much better grasp on the language now, which will hopefully come in handy when writing scripts for personal use.

For the `mismatch` program, I get the sense that my solution may be a bit over-engineered, at least for C. In a higher-level language, using a hash set is exactly the approach I would take, but including all the necessary functions has cluttered the code quite significantly. However, much of it is abstracted away and easy to understand by looking at only the top-level functions. Finding an acceptable hash set library was a bit of a challenge as well, with my first choice not working at all with strings. That said, implementing my own hash set would almost certainly qualify as an over-engineered solution. 

Handling trailing newlines was a bit of a time sink, requiring some creative solutions and repeated code in order to meet the specification.

# Time expenditure
* 2 hr: preliminary research & planning, design doc
* 6 hr: initial implementations
* 2 hr: debugging, revising
* 1 hr: writing tests
* 0.5 hr: self-reflection