#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
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
struct Slot {
    uint32_t tag;
    bool valid, dirty;
    uint32_t load_ts, access_ts;
};

struct Set {
    std::vector<Slot> slots;
};

struct Cache {
    std::vector<Set> sets;
};

//block offset is memory address mod 2n (offset based on size of blocks in the memory (length of an element stored))
int main (int argc, char *argv[])  {
    if (
        ceil(log2(std::atoi(argv[1]))) != floor(log2(std::atoi(argv[1]))) // check if num sets is power of 2
    ) {
        std::cout << argv[1] << "\n";
        std::cout << std::atoi(argv[1]) << "\n";
        std::cerr << "Number of sets is not a power of 2\n";
        return 1;
    }
    else if (ceil(log2(std::atoi(argv[2]))) != floor(log2(std::atoi(argv[2])))) { // check if num of blocks is a power of 2
        std::cerr << "Number of blocks is not a power of 2\n";
        return 1;
    }
    else if (ceil(log2(std::atoi(argv[3]))) != floor(log2(std::atoi(argv[3])))) { // check if block size is a power of 2
        std::cerr << "Block size is not a power of 2\n";
        return 1;
    }
    else if (std::atoi(argv[3]) < 4) {  // check if block size is at least 4
        std::cerr << "Block size is not greater than or equal to 4\n";
    }
    else {
        std::string arg4(argv[4]);
        std::string arg5(argv[5]);
        std::string arg6(argv[6]);
        if (arg4 != "write-allocate" && arg4 != "no-write-allocate") {
            std::cerr << "Invalid argument, must be write-allocate or no-write-allocate\n";
            return 1;
        }
        if (arg5 != "write-through" && arg5 != "write-back") {
            std::cerr << "Invalid argument, must be write-through or write-back\n";
            return 1;
        }
        if (arg6 != "lru" && arg6 != "fifo") {
            std::cerr << "Invalid argument, must be lru or fifo\n";
            return 1;
        }
        if (arg5 == "write-back" && arg4 == "no-write-allocate") {
            std::cerr << "write-back and no-write-allocate cannot be used at the same time\n";
            return 1;
        }
        std::cout << argv[4] << "\n";
        std::cout << argv[5] << "\n";
        std::cout << argv[6] << "\n";
        
    }
    uint32_t num_sets = std::atoi(argv[1]);
    uint32_t num_blocks = std::atoi(argv[2]);
    uint32_t block_size = std::atoi(argv[3]);

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