Overall, the implementation of this program went smoothly. It was surprisingly challenging to think of five ways to accomplish the same task, so I did need to get a little creative with some functions.

# Challenges

One thing that tripped me up was how pass-by-value works with pointers. In `reverse_arr3`, I was attempting to overwrite the pointer to the input array with a new pointer, but found that when the function returned, the original array was untouched. After some research, I found that while the underlying data is mutable, the pointer itself passed as an argument is a copy, and modifying it will have no effect. So, I decided to simply copy the reversed elements to the input array one by one.

The stack function turned out to be a little more cumbersome than I had planned, requiring several helper functions to work properly. Despite that, I believe my implementation is still intuitive.

# Changes

I had initially planned to tackle the Unicode cases but decided against it, as it would require too many tweaks to my existing functions for it to be worth it.

# Time costs

The time I spent on this assignment can be broken down as follows:

- 1 hr. for design and reflection
- 1 hr. for implementation
  - 45 min. for reverse functions
  - 15 min. for command line argument handling and `build.sh`
- 2 hr. for testing, debugging, and revising