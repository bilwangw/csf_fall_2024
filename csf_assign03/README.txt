Bill Wang
Ryan Wu

We worked together to pair program and complete this assignment. We sat together and worked through how to write our program.
We did independently study/research how caches worked, but afterward we shared our knowledge to construct our simulator.
The work split was about even, with both of us contributing various functions to the simulator and also debugging
each others' code.

To determine our simulator effectiveness, we ran tests of various traces and adjusted the settings. 
We measured metrics such as hit rate, miss penalty, and cache size. To track all three metrics simultaneously 
and make comparison easier, we multiplied miss penalty, cycles, and cache size together to try to optimize the 
lowest value possible, which would indicate a generally smaller and faster simulator. We kept the block size 
the same for all tests, reasoning that this value would be determined by the memory trace, not necessarily the 
cache configuration.
The first cache we tested was an approximately even mix of loads and stores. We started with the smallest 
cache size possible, 1 set of 1 block, and compared the performance of the different cache settings (write 
allocate, write through, write back) to determine which performed the best. We immediately noticed that the 
hit rate for this direct mapped cache was extremely bad. We next drastically increased the cache size. We 
noticed that this only increased performance up to a certain point, so we shrunk the cache size down to as 
low as we could go without sacrificing performance. We also tested LRU vs FIFO, and found that in general, 
LRU performed slightly better. Our optimal settings for this cache were 8 sets of 16 blocks, write 
allocate, write back, and LRU.
Once we achieved a very high performance for one cache, we tried testing these settings on another cache. 
This cache had about ten times as many loads as stores. Our previous best cache configuration turned out to 
not be optimal for this cache. Instead, we had 8 sets, 8 blocks, write allocate, write through, and LRU. We 
reasoned this was because this cache had much more loads than stores, so the performance improvement of 
write-back was negligible. A smaller cache was sufficient because we did not have as much data being accessed. 
Testing this cache configuration on the original cache resulted in poor performance, so we needed to find a 
middle ground. Our original configuration did fine for this second cache, and its performance was still 
relatively high (90% hit rate, which is tied for the highest hit rate for the second cache, and a 
relatively low amount of cycles).

The best overall effectiveness we arrived at was:
8 sets, 16 blocks, block size of 4, write allocate, write back, LRU.
Note that block size was constant for all of our tests.
