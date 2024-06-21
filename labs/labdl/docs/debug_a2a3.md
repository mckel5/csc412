# Debug Old Code (A2 & A3)

### Group Member's Name
[partner]

#### Program Name (A2 or A3)
Head.cpp - A3
#### Stack Layout

Show how local variables are allocated on the stack by using gdb after running your program.
```
$ (gdb) bt
```
Copy and paste the output here.

```
#0  __libc_start_call_main (main=main@entry=0x555555555229 <main>, argc=argc@entry=3, argv=argv@entry=0x7fffffffdb08) at ../sysdeps/nptl/libc_start_call_main.h:74
#1  0x00007ffff7b8ae40 in __libc_start_main_impl (main=0x555555555229 <main>, argc=3, argv=0x7fffffffdb08, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7fffffffdaf8) at ../csu/libc-start.c:392
#2  0x0000555555555165 in _start ()
```

#### Improvements and Debugging

**Describe the bugs you found?**

I found no bugs within my program

**Describe any unsafe code you found?**

I found a possible improvement with the allocation of a vuffer for 256 bytes.
While this is okay for smaller paths, there are situations where path length exceeds this limit.

**Describe one improvement made to the code**

To change what could be improved, I can dynamicall allocated the buffer by increasing the buffer size by 2x if there existed an error 'ERANGE'.

#### Fixing a Bug or Improving the Code

Copy your original code as NAME_original.c or NAME_original.cpp.

Implement a fix or improvement identified above. What lines was it implemented?

The entire program was changed in some way, with the reordering of certain lines.
but the main magic of the "dynamically allocated buffer" is within lines 15 through 17

### Group Member's Name

Marceline Kelly

#### Program Name (A2 or A3)

A2: makewords.c

#### Stack Layout

Show how local variables are allocated on the stack by using gdb after running your program.
```
(gdb) bt -full
#0  makewords (input=0x7fffffefe160 "the quick brown fox jumped over the lazy dog\n") at makewords.c:37
        maxLen = 46
        result = 0x55555555a2b0 "the\nquick\nbrown\nfox\njumped\nover\nthe\nlazy\ndog\n"
#1  0x000055555555531a in main () at makewords.c:47
        result = 0x0
        buffer = <error reading variable buffer (value requires 1048576 bytes, which is more than max-value-size)>
```
Copy and paste the output here.

This code is currently paused at the end of my "makewords" function.


#### Improvements and Debugging

**Describe the bugs you found?**

I noticed that AddressSanitizer was reporting a "heap buffer overflow" error when I added a null terminator to my result string.
It turns out that I was setting the character just after the end of the buffer to '\0', not the last character itself.
Interestingly, the program ran fine while this bug was present.

**Describe any unsafe code you found?**

This bug could have resulted in some data corruption, though evidently the potential was minor as I never ran into any issues while testing the original program.

**Describe one improvement made to the code**

To fix this issue, I simply changed

```c
result[maxLen] = '\0';
```

to


```c
result[maxLen - 1] = '\0';
```

#### Fixing a Bug or Improving the Code

Copy your original code as NAME_original.c or NAME_original.cpp.

Implement a fix or improvement identified above. What lines was it implemented?

Line 35, in `makewords()`