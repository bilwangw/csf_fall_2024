Bill Wang
Ryan Wu

We both worked on the project together, brainstorming ways to implement the fork. We both wrote the framework
of the code, then later filled it in and debugged.

REPORT

OUTPUT:
---------------------------------
Test run with threshold 2097152

real    0m0.382s
user    0m0.367s
sys     0m0.009s
Test run with threshold 1048576

real    0m0.241s
user    0m0.392s
sys     0m0.031s
Test run with threshold 524288

real    0m0.172s
user    0m0.431s
sys     0m0.036s
Test run with threshold 262144

real    0m0.143s
user    0m0.450s
sys     0m0.046s
Test run with threshold 131072

real    0m0.141s
user    0m0.457s
sys     0m0.051s
Test run with threshold 65536

real    0m0.125s
user    0m0.471s
sys     0m0.056s
Test run with threshold 32768

real    0m0.125s
user    0m0.469s
sys     0m0.094s
Test run with threshold 16384

real    0m0.113s
user    0m0.491s
sys     0m0.091s
----------------------------------

We noticed a trend where as the threshold value decreases, the program runtime also decreases.
This makes sense because a lower threshold means the program does more iterations of the sorting
in parallel using different CPU cores. At higher thresholds, the program will not be utilizing
parallelism as much, so the program will take longer. As the threshold got lower, the program time decrease 
saw diminishing returns. This makes sense because as the thresholds get smaller, the time it
would take for a single CPU to sort the data would be comparable to processing the data in parallel.