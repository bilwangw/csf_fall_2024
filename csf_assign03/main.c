#include <math.h>
#include <iostream>

// ./csim 256 4 16 write-allocate write-back fifo < gcc.trace

/* inputs
number of sets in the cache (a positive power-of-2)
number of blocks in each set (a positive power-of-2)
number of bytes in each block (a positive power-of-2, at least 4)
write-allocate or no-write-allocate
write-through or write-back
lru (least-recently-used) or fifo evictions
*/

/* invalid input
block size is not a power of 2
number of sets is not a power of 2
block size is less than 4
write-back and no-write-allocate were both specified
*/

int main (int argc, char *argv[])  {
    if (
        ceil(log2(int(*argv[1]))) != floor(log2(int(*argv[1]))) // check if num sets is power of 2
    ) {
        std::cerr << "Number of sets is not a power of 2\n";
        return 1;
    }
    else if (ceil(log2(*argv[2])) != floor(log2(*argv[2]))) { // check if num of blocks is a power of 2
        std::cerr << "Number of blocks is not a power of 2\n";
        return 1;
    }
    else if (ceil(log2(*argv[3])) != floor(log2(*argv[3]))) { // check if block size is a power of 2
        std::cerr << "Block size is not a power of 2\n";
        return 1;
    }
    else if (*argv[3] < 4) {  // check if block size is at least 4
        std::cerr << "Block size is not greater than or equal to 4\n";
    }
    else if (
        ((argv[4] != "write-allocate") && (argv[4] != "no-write-allocate")) // check if arguments are correct
        || ((argv[5] != "write-through") && (argv[5] != "write-back")) // check if arguments are correct
        || (argv[6] != "lru" && argv[6] != "fifo") // check if arguments are correct
    ) {
        std::cerr << "Invalid arguments\n";
        return 1;
    }
    else if ((argv[5] == "write-back" && argv[4] == "no-write-allocate")) { // check condition
        std::cerr << "write-back and no-write-allocate cannot be used at the same time\n";
        return 1;
    }
    else {
        
    }

    return 0;
}

/* output
Total loads: count
Total stores: count
Load hits: count
Load misses: count
Store hits: count
Store misses: count
Total cycles: count
*/