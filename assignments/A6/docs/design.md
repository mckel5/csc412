# A6 Design Document

Marceline Kelly

## Strategy

### Statistics

Maintain a global variable referencing a `dmalloc_stats` object that `dmalloc()` and `dfree()` update, as needed.
When `get_statistics(dmalloc_stats* stats)` is called, simply `memcpy()` this global struct to `stats`.
To record the amount of heap memory allocated, `dfree()` will insert a `block_metadata` object (as described in the ["Data structures"](#data-structures) section) into each newly-allocated block of memory.
`dmalloc()` and `dfree()` will use pointer arithmetic and the `sizeof` function to retrieve the true size of a block when performing memory operations.

### Integer overflow protection

Perform the appropriate multiplication (`nmemb * sz`), then divide the product by `nmemb`.
If the result is `sz`, continue as normal.
Otherwise, an overflow has occurred; return `nullptr` and increment the failed allocation statistic.

### Invalid free and double-free detection

Maintain a set containing the addresses of all currently-allocated memory blocks (specifically, the addresses of their payloads).
A successful call to `dmalloc()` adds an address to this set.
When `dfree()` is called, check if the desired address exists in the set.
If it does, delete the set entry and free the specified memory.
If it does not, print an error and abort.
This protects against both invalid and double free scenarios.

### Boundary write error detection

In `dmalloc()`, reserve four bytes of memory immediately before and after the payload.
Fill these spaces with a predefined, constant value.
When `dfree()` is called, check if the values in these spaces match the predefined value.
If there is a mismatch, an out-of-bounds write likely occurred; print an error message.
There is a very small chance that the user manages to write data to these spaces that matches the predefined value, in which case no error will be raised.

### Memory leak reporting

In `print_leak_report()`, iterate over the allocated memory set.
Any addresses in the set at this point must not been freed.
Using the metadata struct, print the location and size of each of these blocks.

### Advanced reports and checking

In `dmalloc()`, in addition to checking whether the address to be freed exists in the allocation set, check if it exists within any existing block.
To do this, iterate over each address in the set, calculate the start/end addresses using metadata + pointer arithmetic, and test whether the problematic address lies between these bounds.
Additionally, check if a valid metadata object actually exists near the address in question.
Check if the address reported in the set matches that listed in the metadata.
This is to help thwart diabolical free scenarios.

## Obstacles

### `gdb` inexperience

Apart from the double-link lab, I have never used `gdb` in its command-line form.
Typically, I will use it through the graphical debugger in VS Code, but this is not always possible, such as when running programs in the Docker environment.
To prepare myself for this assignment, I will learn more about how to effectively use `gdb` to debug programs.
I'll also see if there's a way to run the VS Code debugger in the Docker environment.

### `malloc()` allocation patterns

Learning to read `gdb` output is useful when encountering a bug, but I think it would serve me well to learn how `malloc()` allocates memory on the heap.
Specifically, I want to know if the address chosen for the new block is undefined or if there is a pattern.
For instance, if I call `malloc()` twice back-to-back, will the two blocks be allocated sequentially on the heap?
This could help me catch some sneaky edge cases while implementing `dmalloc()` and `dfree()`.

## Outline

### Functions

I do not plan to add any functions to the stencil code.

### Data structures

Whenever `dmalloc()` or `dfree()` is called, it will update a global instance of the `dmalloc_stats` struct described in the handout.
`get_statistics()` will retrieve these statistics when it is called.

```cpp
dmalloc_stats g_statistics = { ... };   // Default initialization
```

All dynamically-allocated memory addresses will be added to a set called `active_addresses`.
When these addresses are freed, they will likewise be removed from the set.

```cpp
std::unordered_set<size_t> g_active_addresses;
```

Each memory block allocated with `dmalloc()` will begin with a `block_metadata` object, which describes useful information about the memory in question.

```cpp
struct block_metadata {
    size_t addr;
    size_t size;
};
```