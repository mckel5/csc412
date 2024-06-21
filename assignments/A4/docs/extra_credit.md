## Change 3: Prevent grid cell duplication when multiple inklings occupy the same cell


### Proposed Change Summary

When two inklings occupy the same grid cell, small visual glitches occur.
This issue can be eliminated by ensuring that only one inkling is drawn per cell.

### Describe the Problem

When running the program, some rows in the grid occasionally get "shifted" to the right when printed to the terminal.
This happens when two inklings occupy the same cell.
There is no check for this, so both inklings are printed adjacent to each other, thus shifting the rest of the row.

### Describe the Solution

To fix this bug, we need to keep track of whether an inkling has already been drawn at the current cell.
If another inkling is found at that location, it will not be drawn.

### Describe the Implemented Solution

At each grid cell, the drawing function loops through each inkling to determine if any reside at the current cell.
If one is found, I simply break out of the inkling loop after drawing, thus ensuring that only one inkling is drawn per cell.

### Describe how you Tested your Implemented Solution

I ran the program with a large number of inklings (20+) to increase the likelihood of cell collisions.
After implementing my fix, I didn't notice any further row shifting.