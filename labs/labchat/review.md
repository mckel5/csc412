## Expectations and Plan

Going into this assignment, my plan was to provide ChatGPT with the handout instructions and starter code before asking it to complete each TODO comment. It turns out that I didn't even need to ask it to create an implementation, which it began to do as soon as I pasted in the starter code. I expected that it would output code that vaguely resembled what the assignment called for but ultimately failed to run properly.

## Code Review (you review the code quality)

ChatGPT was able to accurately implement FIFO and Round Robin simulations, as far as I can tell. I'm not entirely sure if it needed as many lines of code as it used to do so, but it does work, at least. However, I couldn't get it to correctly implement the metrics for each algorithm. When executed, the program reports negative time and "infinity" processes run per second. I tried asking Chat to fix this, but realized that I probably wasn't going to get anywhere since it seemed like it was referencing whatever variables it felt like in these calculations. After some prodding, however, I was able to get the code to compile without any warnings.

## Self-Reflection

Overall, I was pleasantly surprised by how comprehensive of a solution Chat provided me with. The program wasn't complete on the first try, but as I asked Chat to tweak things to conform to the handout, I felt it understood what I was asking remarkably well. Still, I ran into some hurdles (particularly with the metrics) that I think would have taken more time to coax Chat into fixing than it would have to just modify the code by hand.

## Compare Algorithms

The metric code in my program isn't working, so I manually timed each algorithm's running time:

FCFS: 26.51 s

RR: 39.97 s

In my program, RR used mutex locks and time quantums, while FCFS did not due to the latter's simpler nature. In this case, it appears these features slowed down the execution of the processes quite significantly. However, in a real-world scenario without dummy sleep statements, it's possible that this difference would not be so drastic. I'd even predict that RR would be faster overall when executing many processes at once.

## Technical Concepts

Mutex locks are essential for preventing race conditions in multithreaded applications. [Baeldung](https://www.baeldung.com/cs/race-conditions) provides a simple yet apt example:

Imagine a buggy banking app that allows users to transfer money to other users. Let's say User A wants to send $300 to User B. If User A manages to accidentally send multiple requests to the server at the same time, then without proper state locks, User A will now have sent $600, $900, or potentially even more to User B. User A might not even have that much money in their account, but the app's checks for this are flawed and allowed the transfers anyway.

In a field like finance where accuracy and reliability are paramount, the lack of proper state management can have disastrous results, especially if Users A and B aren't people but, say, multinational corporations.