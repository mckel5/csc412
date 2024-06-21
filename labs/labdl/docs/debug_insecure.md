# Debug Insecure Code

### Stack Trace

##### Results with gets()
```
(gdb) bt full
#0  main (argc=1, argv=0x7fffffffe5e8) at main.c:36
        shared_memory = 0x7ffff7159000 "\005"
        buffer = "\352\224W\367\377\177\000\000\005\000\000\000p`\000"
        pid = 0
(gdb) p m
$3 = 22
(gdb) n
Enter some text: appleorangebananawatermelongrapecherrylemon
41              exit(0);
(gdb) bt full
#0  main (argc=1, argv=0x7fffffffe5e8) at main.c:41
        shared_memory = 0x7ffff7159000 "\005"
        buffer = "appleorangebanan"
        pid = 0
(gdb) p m
$4 = 22
```

##### Results with fgets()
```
(gdb) bt full
#0  main (argc=1, argv=0x7fffffffe5e8) at main.c:39
        shared_memory = 0x7ffff7159000 "\005"
        buffer = "\352\224W\367\377\177\000\000\005\000\000\000p`\000"
        pid = 0
(gdb) p m
$1 = 22
(gdb) n
Enter some text: appleorangebananawatermelongrapecherrylemon
41              exit(0);
(gdb) bt full
#0  main (argc=1, argv=0x7fffffffe5e8) at main.c:41
        shared_memory = 0x7ffff7159000 "\005"
        buffer = "appleorangebana"
        pid = 0
(gdb) p m
$2 = 22
```

##### Describe the differences between the stack traces

There is no difference between these stack traces, except for the value of `buffer`.

### Develop your Intution

Answer the questions below as a group.

##### Describe steps on how to recreate to a buffer overflow when gets() is used.

When entering input, note the size of the buffer `gets()` is writing its result to, and enter any amount of data greater than that buffer size.

##### Describe the change in values of buffer, shared_memory, and m as a result of overflow.

`buffer` contains the first sixteen characters of the user's input. It changes depending on what was entered, but cuts off the input whether the user was finished or not.
Neither `shared_memory` nor `m` changed between runs.
I assume they were supposed to get overwritten by `gets()`, but this did not happen in our testing.

##### Provide a short intuitive explanation of how gets() causes an error and how fgets() fixes this.

`gets()` simply copies until the end of the stream is reached, regardless of the capacity of the destination.
`fgets()` puts a limit on how much data it will copy to the destination to prevent writing into adjacent memory.

##### Provide a short intuitive explanation of how GDB to analyze programs with parent/child processes and shared_memory.

* If you've identified a possible area of unsafe code, set breakpoints before and after the code in question.
  Then, see if any variables get corrupted if you purposefully supply garbage data to the program.
* Set the flag `follow-fork-mode` to `child` to ensure that GDB stops at breakpoints in child processes.
* When examining the stack trace, be sure to run `bt full` to show local variables, as they will not be shown otherwise.