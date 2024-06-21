# Successes

Overall, I was able to follow my original design closely while implementing this program.
Having my design doc on hand helped a lot as I was programming.
I was able to view each task from a high level and avoid becoming confused while working with manual memory management.

I enjoyed the challenge of implementing `drealloc`.
My plan was to resize the provided memory block, if possible, and allocate a larger memory block otherwise.
In hindsight, I think it would have been fine to always reallocate new memory, but regardless, I'm glad I took the time to determine how resizing memory blocks should work.

The C++ integration tests (035-039) passed on the first try when I got to them, which was great to see.

# Obstacles

One tricky pitfall that I consistently fell into was miscalculating memory offsets.
In my `dmalloc` implementation, each heap block contains metadata, alignment padding, and out-of-bounds write detection data, in addition to the actual memory space exposed to the user.
Often I would forget to account for one of these parts when moving memory around, leading to some important data getting overwritten and requiring a lot of debugging to figure out where the resulting values were coming from.

Additionally, tweaking my code to pass one test would occasionally cause earlier tests to fail.
As a result, I would have to go back and rethink my solutions to earlier tasks to ensure that they covered edge cases I hadn't considered.

# Changes

In my design document, I planned to create a set of all actively-allocated memory addresses, which would help identify invalid and double free errors.
However, it turned out that this approach didn't help with double-frees, since any freed address would automatically be removed from the set.
The program would then have no way of knowing whether a block had already been freed.
To fix this, I initially added a "freed" flag to the metadata that `dfree` would check.
After some testing, I decided to remove this flag and instead maintain a set of freed addresses.
This prevents undefined behavior caused by maliciously overwriting the flag.

# Research process

One unintuitive aspect of C++ that I had to look up was how to check for set membership.
Older versions of the language do not include a `contains` method for sets, so I needed to use the `find` method as a workaround.

I also took the time to research how casts work in C++.
C-style casts would have worked fine for this assignment, I think, but I knew that C++ had a more robust type conversion system and I wanted to do things the "proper" way.
I ended up using `reinterpret_cast` exclusively since working with `malloc` involves a lot of handling of `void` pointers.

One thing I found interesting while performing research is that assignments in `if` statement conditions are allowed.
I had always thought that the compiler would mark this as an error, since most people probably mean to use `==` instead of `=`.
As it turns out, though, the assignment actually does return the right-hand value, which the `if` statement can then check.
It doesn't change the structure of the code much but is a handy shortcut to know.

# Time expenditure

- Design doc: 1 hour
- Implementation
  - Statistics: 1 hour
  - Integer overflow protection: 15 minutes
  - Invalid free and double-free detection: 45 minutes
  - Boundary write error detection: 30 minutes
  - Memory leak reporting: 30 minutes
  - Advanced reports and checking: 30 minutes
- Extra credit: 1 hour
- Reflection: 1 hour

TOTAL (approx.): 6.5 hours


