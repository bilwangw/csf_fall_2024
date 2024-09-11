Bill Wang, Ryan Wu

We split the work evenly. Ryan mainly did the constructors
destructor, assignment operator, git_bits, and Bill mainly 
did get_bit_vector, is_negative, operator-, to_hex. 
For milestone 2 we collaborated for each part. Both of us
met up and programed on one computer while we worked through
the solution. Our subtraction operator is wholly dependent 
on our addition operator, so any edge cases for subtraction
would also apply to addition, so we decided not to focus on
testing the subtraction function for edge cases.
Our multiplication operator follows the algorithm
provided by the assignment write-up. We decided not to spend 
our time implementing division. Our to_dec implementation
was left unfinished for similar reasons. We reasoned that
our efforts were better spent debugging the rest of the code.

We wrote a test case for unary minus. We added two helper 
functions: one to test for overflow, and one to test if a 
BigInt was zero.