#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <map>
#include <tuple>
// ./csim 256 4 16 write-allocate write-back fifo < traces/hits.trace
//256 sets, 4 blocks, 16 bits in each block (offset size)

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
    std::map<uint32_t, Slot *> index;
};

struct Cache {
    std::vector<Set> sets;
};

//global variables
uint32_t currentTime = 0;

uint32_t loads = 0;
uint32_t stores = 0;
uint32_t load_hits = 0;
uint32_t load_misses = 0;
uint32_t store_hits = 0;
uint32_t store_misses = 0;
uint32_t cycles = 0;

std::tuple<uint32_t, uint32_t> parseAddress(uint32_t address, Cache cache, uint index_len, uint offset_len) {
    currentTime++; // increment time stamp
    uint32_t index;
    uint32_t tag;
    index = (address%(2<<(offset_len + index_len))) >> offset_len;
    tag = address >> (offset_len + index_len);
    return std::make_tuple(tag, index);
}

void writeToSlot(Cache cache, uint index, uint32_t tag, uint slot) {
    cache.sets[index].slots[slot].valid = false;
    cache.sets[index].slots[slot].dirty = true;
    cache.sets[index].slots[slot].tag = tag;
    cache.sets[index].slots[slot].load_ts = currentTime;
    cache.sets[index].slots[slot].access_ts = currentTime;
}

void overwriteSlot(Cache cache, uint index, uint32_t tag, uint slot) {
    cache.sets[index].slots[slot].valid = false;
    cache.sets[index].slots[slot].dirty = true;
    cache.sets[index].slots[slot].tag = tag;
    cache.sets[index].slots[slot].access_ts = currentTime;
}

void loadBlock(uint32_t address, Cache cache, uint index_len, uint offset_len, bool lru_fifo) {
    uint32_t tag, index;
    std::tuple<uint32_t, uint32_t> parsedAddress;
    parsedAddress = parseAddress(address, cache, index_len, offset_len);
    tag = std::get<0>(parsedAddress);
    index = std::get<1>(parsedAddress);
    if(cache.sets[index].slots.size() == 1) { // direct mapped case
        if(cache.sets[index].slots[0].valid == true) { // if slot is valid, then it is empty
            load_misses++;
            cycles++;
            writeToSlot(cache, index, tag, 0); // so block is empty, so load from mem
            cycles+=2<<(offset_len-3)*25; // 100 cycles per 4 bytes being transferred, so 2^offset_len bits, divide by 8 (to get bytes), times 25 cycles/byte
        }
        else {
            load_hits++;
            cycles++;
            overwriteSlot(cache, index, tag, 0); // successful hit, overwrite to update time stamp
        }
    }
}

void storeBlock(uint32_t address, Cache cache, uint index_len, uint offset_len, bool wAlloc, bool wBackThru, bool lru_fifo) {
    uint32_t tag, index;
    std::tuple<uint32_t, uint32_t> parsedAddress;
    parsedAddress = parseAddress(address, cache, index_len, offset_len);
    tag = std::get<0>(parsedAddress);
    index = std::get<1>(parsedAddress);
    if(!wAlloc && wBackThru) {// no-write-allocate and write-through
        if(cache.sets[index].slots.size() == 1) { // check direct mapped case
            if(cache.sets[index].slots[0].valid == false) { // if slot is invalid, this is a miss (slot in use)
                store_misses++;
                cycles++;
            }
            else { // if slot empty, fill it
                writeToSlot(cache, index, tag, 0);
                store_hits++;
                cycles++;
            }

        }
    }
}


//block offset is memory address mod 2n (offset based on size of blocks in the memory (length of an element stored))
int main (int argc, char *argv[])  {
    std::string arg4(argv[4]);
    std::string arg5(argv[5]);
    std::string arg6(argv[6]);
    if (
        ceil(log2(std::atoi(argv[1]))) != floor(log2(std::atoi(argv[1]))) // check if num sets is power of 2
    ) {
        // std::cout << argv[1] << "\n";
        // std::cout << std::atoi(argv[1]) << "\n";
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
        
    }
    bool wAlloc; // false is no-write-allocate, true is write-allocate
    bool wBackThru; // false is write-back, true is write-through
    bool lru_fifo; // false is lru, true is fifo

    if (arg4.compare("write-allocate")) {
        wAlloc = 1;
    }
    else if (arg4.compare("no-write-allocate")) {
        wAlloc = 0;
    }
    if (arg5.compare("write-through")) {
        wBackThru = 1;
    }
    else if (arg5.compare("write-back")) {
        wBackThru = 0;
    }
    if (arg6.compare("lru")) {
        lru_fifo = 0;
    }
    else if (arg6.compare("fifo")) {
        lru_fifo = 1;
    }

    uint32_t num_sets = std::atoi(argv[1]);
    uint index_len = log2(num_sets); // get the bit length of sets
    uint32_t num_blocks = std::atoi(argv[2]);
    uint32_t block_size = std::atoi(argv[3]);
    uint offset_len = log2(block_size);
    uint tag_len = 32 - offset_len - index_len; // compute tag length by subtracting offset and index length from the length of the memory address
    std::vector<Set> sets(num_sets);


    std::vector<Set>::iterator it;
    int i;

    Slot blank_slot;
    blank_slot.valid = true;
    blank_slot.dirty = false;
    for(it = sets.begin(); it != sets.end(); it++,i++ )    {
        std::vector<Slot> slots(num_blocks);
        it->slots = slots;
    }


    Cache cache;
    cache.sets = sets;

    char op; // operation (load or store)
    std::string address; // memory address
    char dummy; // dummy variable to store third argument, necessary for iterating through cin


    while(std::cin >> op) {
        std::cin >> address;
        //std::cout << address << "\n";
        std::cin >> dummy;
        if (op == 'l') {
            loadBlock(stoi(address), cache, index_len, offset_len, lru_fifo);
            loads++;
        }
        else if (op == 's') {
            storeBlock(stoi(address), cache, index_len, offset_len, wAlloc, wBackThru, lru_fifo);
            stores++;
        }
        
    }

    std::cout << "Total loads: " << loads << "\n";
    std::cout << "Total stores: " << stores << "\n";
    std::cout << "Load hits: " << load_hits << "\n";
    std::cout << "Load misses: " << load_misses << "\n";
    std::cout << "Store hits: " << store_hits << "\n";
    std::cout << "Store misses: " << store_misses << "\n";
    std::cout << "Total cycles: " << cycles << "\n";
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