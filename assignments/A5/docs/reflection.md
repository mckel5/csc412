# Successes

Adding a linked list to my Snake struct was difficult, but I managed to do it.
Initially, I tried to change the type of the list from `void*` to `int` before realizing that doing so would require me to essentially rewrite all the associated functions.
Then, I tried storing pointers to individual cells in the list.
This probably would have worked had I given it some more time, but the complexity of this approach began to quickly get out of hand, so I scrapped my code and started again.
Eventually, I got the list working by storing simple integers, which represented indices into the board array.
I was able to do this with fewer casts than I had expected, so I'm pleased with the end result.

# Obstacles

One of the most difficult parts of this assignment for me was, interestingly, allowing the snake to move into the space previously occupied by its tail.
I reorganized my logic for updating the snake's position over and over but nothing seemed to work.
When tracing the code line by line, everything made sense in my head, and I was having a lot of trouble figuring out why the game kept ending prematurely.
Eventually, after a lot of analysis, I was able to figure out a set of conditional checks to handle this edge case, though my solution isn't as elegant as I hoped it would be.

Handling Unicode characters was also a challenge; to check for combination bytes (`10xxxxxx`), I would simply bit shift the number right by six digits then check if the result equaled `00000010`.
This caused some strange behavior, and after a considerable amount of trial and error, I determined why: the `char` type is signed by default.
This meant a one would be prepended to the byte for each right shift, not a zero.
I was able to solve this problem by interpreting the user's input as unsigned `chars`.

# Changes

For the `cells` array, I thought at first that I would need to manipulate the flags with bitwise operations.
This wasn't necessary, however, as in this version of the game, no cell will have multiple flags set.
This allowed me to just overwrite cell types without impacting functionality.

Other than this and the changes to the linked list mentioned above, I didn't have to make many changes to my initial plan, since the handout was thorough in describing what was expected of me.

# Research process

Learning how to use void pointers took quite a while.
I tried to look for examples of how others have used them in their own code (like on StackOverflow), but many of the examples were just different enough from my own use case that I found them more confusing than anything.
I'm also used to languages like C++ with dedicated generic types, so I had to change my way of thinking in order to successfully complete this assignment.
While I have gotten a better handle on this concept, I can't say I'm any more excited about the prospect of working with C in the future.

# Time expenditure

- Design document: 1 hour
- Research: 2 hours
- Implementation: 4 hours
- Debugging: 2 hours
- Reflection: 1 hour

TOTAL: approx. 10 hours
