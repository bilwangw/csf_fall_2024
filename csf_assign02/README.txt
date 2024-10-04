Bill Wang
Ryan Wu

Milestone 1:

Both of us worked together on the functions. One of us would type out the code while both of us
thought of how to implement it. However, debugging was generally done independently. We both
made the helper functions.
We made helper functions for getting the rgba values from a pixel, converting to grayscale, converting to composite, 
and making a pixel from rgba values.


Milestone 2:

Both of us worked together (pair programming style) on the assembly code. We had a tough time starting out since we didn't have 
a good sense of the register conventions (were using caller saved registers across function calls). Furthermore, a small error
in the way we stored the input and output pointers caused repeated segmentation faults that took a long time to fix. It took us 
a while to get used to the hang of assembly and we struggled often with small errors (using r13d and not r13 which truncated 
addresses when overflow 4 bytes).

Milestone 3:

Once again, we worked together on implementing functions. Initially, we had a lot of bugs with our code. But after much 
troubleshooting, we got composite to work. We decided that our efforts were better spent on fully implementing composite,
so we did not complete tile. Overall, the implementation of composite went much smoother than the implementation of Milestone
2, as we had a much better grasp of assembly now.