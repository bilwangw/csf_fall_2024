/* Cache Simulator
CSF Assignment 3
Ryan Wu - rwu42@jh.edu
Bill Wang - bwang89@jh.edu

Cache simulator written using C++

Inputs:
number of sets in the cache (a positive power-of-2)
number of blocks in each set (a positive power-of-2)
number of bytes in each block (a positive power-of-2, at least 4)
write-allocate or no-write-allocate
write-through or write-back
lru (least-recently-used) or fifo evictions

Outputs:
Total loads: count
Total stores: count
Load hits: count
Load misses: count
Store hits: count
Store misses: count
Total cycles: count
*/

/* invalid input
block size is not a power of 2
number of sets is not a power of 2
block size is less than 4
write-back and no-write-allocate were both specified
*/
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <tuple>
#include <bitset>

//definition of structs to model the cache 
struct Slot {
    uint32_t tag;
    bool valid, dirty;
    uint32_t load_ts, access_ts;
    Slot() : tag(0), valid(false), dirty(false), load_ts(0), access_ts(0) {}
    ~Slot() {

    }
};

struct Set {
    std::vector<Slot> slots;
    ~Set() {

    }
};

struct Cache {
    std::vector<Set> sets;
    ~Cache() {

    }
};

//global variables
uint64_t currentTime = 0;

uint32_t loads = 0;
uint32_t stores = 0;
uint32_t load_hits = 0;
uint32_t load_misses = 0;
uint32_t store_hits = 0;
uint32_t store_misses = 0;
uint32_t cycles = 0;
uint32_t cycle_mult;


void writeToMap(Cache &cache, uint32_t index, uint32_t tag, bool lru_fifo, bool wBackThru) {
    //find oldest block in slots vector
    uint32_t oldest = 4294967295; //initialize oldest timestamp to max unsigned int value
    int best_index = 0;
    for (size_t i = 0; i < cache.sets[index].slots.size(); i++) {
    // first check if slot is invalid, if so that means it is empty and can be used immediately
        if(cache.sets[index].slots[i].valid == false) {
            best_index = i;
            break;
        }
        // if slot is filled, check access or load timestamp depending on LRU or FIFO and compare with current oldest found
        if(lru_fifo) {
            if (oldest > cache.sets[index].slots[i].load_ts) {
                oldest = cache.sets[index].slots[i].load_ts;
                best_index = i;
            }
        }
        else {
            if (oldest > cache.sets[index].slots[i].access_ts) {
                oldest = cache.sets[index].slots[i].access_ts;
                best_index = i;
            }
        }
    }
    
    //increment cycles simulating memory access
    if (cache.sets[index].slots[best_index].dirty && !wBackThru) {
        cycles += cycle_mult * 100;
    }
    //overwrite existing or blank slot
    cache.sets[index].slots[best_index].tag = tag;
    cache.sets[index].slots[best_index].valid = true;
    cache.sets[index].slots[best_index].dirty = true;
    cache.sets[index].slots[best_index].access_ts = currentTime++;
    cache.sets[index].slots[best_index].load_ts = currentTime++;
}
// update access time stamp
void mapUpdateTs(Cache &cache, uint32_t index, uint32_t tag) {
    for(size_t i = 0; i < cache.sets[index].slots.size(); i++) {
        if(cache.sets[index].slots[i].tag == tag) {
            cache.sets[index].slots[i].access_ts = currentTime++;
            return;
        }
    }
}
// check if tag exists in the slots vector
bool tagIsValid(Cache &cache, uint32_t index, uint32_t tag) {
    for(size_t i = 0; i < cache.sets[index].slots.size(); i++) {
        if(cache.sets[index].slots[i].tag == tag) {
            if(cache.sets[index].slots[i].valid == true) {
                return true;
            }
        }
    }
    return false;
}

// simulate loading a block
void loadBlock(Cache &cache, uint32_t tag, uint32_t index, bool wBackThru, bool lru_fifo, uint32_t cycle_mult) {
    // check if tag currently has a slot
    if(tagIsValid(cache, index, tag)) {
        load_hits++;
        mapUpdateTs(cache, index, tag);
        cycles++; // simulate cache access cycle
    }
    else {
        load_misses++;
        //update by writing new data into cache
        writeToMap(cache, index, tag, lru_fifo, wBackThru);
        cycles += 100 * cycle_mult; // simulate memory access cycles
        cycles++; 
    }

}

// simulator storing a block 
void storeBlock(Cache &cache, uint32_t tag, uint32_t index, bool wAlloc, bool wBackThru, bool lru_fifo, uint32_t cycle_mult) {
    if(tagIsValid(cache, index, tag)) {
        store_hits++;
        if (wBackThru) {
            //write straight to memory and cache
            mapUpdateTs(cache, index, tag);
            cycles++; 
            cycles += 100; 
        } else {
            //write only to cache and mark as dirty
            mapUpdateTs(cache, index, tag);
            cycles++; 
        }
    }
    else {
        store_misses++;
        // check if write-allocate
        if (wAlloc) {
            // check if write-through
            if (wBackThru) {
                cycles++;
                cycles += cycle_mult * 100;
                //access memory and write into slot + memory directly
                writeToMap(cache, index, tag, lru_fifo, wBackThru);
            // if write-back
            } else {
                //access memory and overwrite current data in cache to be dirty, do not store in memory only in cache
                writeToMap(cache, index, tag, lru_fifo, wBackThru);
                cycles += cycle_mult * 100;
                cycles++;
            }
        // if no-write-allocate
        } else {
            // only need to check if write-through (no-write-allocate + write-through option not possible)
            if (wBackThru) {
                //write directly to memory no involvement of cache
                cycles += 100;
            }
        }
    }
}

int main (int argc, char *argv[])  {
    // store options to strings
    std::string arg4(argv[4]);
    std::string arg5(argv[5]);
    std::string arg6(argv[6]);
    //error checking for incorrect inputs
    if (ceil(log2(std::atoi(argv[1]))) != floor(log2(std::atoi(argv[1])))) {// check if num sets is power of 2
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
        return 1;
    }
    else if (arg5 == "write-back" && arg4 == "no-write-allocate") {
            std::cerr << "write-back and no-write-allocate cannot be used at the same time\n";
            return 1;
    }
    bool wAlloc; // false is no-write-allocate, true is write-allocate
    bool wBackThru; // false is write-back, true is write-through
    bool lru_fifo; // false is lru, true is fifo

    // store cache configuration options to booleans, parsing input
    if (arg4 == "write-allocate") {
        wAlloc = 1;
    }
    else if (arg4 == "no-write-allocate") {
        wAlloc = 0;
    }
    else {
        std::cerr << "Invalid argument, must be write-allocate or no-write-allocate\n";
        return 1;
    }
    if (arg5 == "write-through") {
        wBackThru = 1;
    }
    else if (arg5 == "write-back") {
        wBackThru = 0;
    }
    else {
        std::cerr << "Invalid argument, must be write-through or write-back\n";
        return 1;
    }
    if (arg6 == "lru") {
        lru_fifo = 0;
    }
    else if (arg6 == "fifo") {
        lru_fifo = 1;
    }
    else {
        std::cerr << "Invalid argument, must be lru or fifo\n";
        return 1;
    }

    uint32_t num_sets = std::atoi(argv[1]);
    uint index_len = log2(num_sets); // get the bit length of sets
    uint32_t num_blocks = std::atoi(argv[2]);
    uint32_t block_size = std::atoi(argv[3]);
    uint offset_len = log2(block_size);
    std::vector<Set> sets(num_sets);

    //initialize all slots
    for(std::vector<Set>::iterator it = sets.begin(); it != sets.end(); it++ )    {
        std::vector<Slot> slots(num_blocks);
        it->slots = slots;
    }

    //define the Cache struct to be used
    Cache cache;
    cache.sets = sets;

    char op; // operation (load or store)
    std::string address; // memory address
    std::string dummy; // dummy variable to store third argument, necessary for iterating through cin

    uint32_t tag;
    uint32_t index;
    cycle_mult = 1 << offset_len;
    cycle_mult /= 4; 

    // Read through the trace file line by line
    while(std::cin >> op) { // store l or s in op
        std::cin >> address; // store memory address
        std::cin >> dummy; // store third number in dummy buffer
        currentTime++; // increment global time

        uint32_t intAddress = stoul(address.substr(2),nullptr,16);
        tag = intAddress >> (offset_len + index_len);
        index = (intAddress >> offset_len) & ((1 << index_len) - 1);
        
        if(op == 'l') {
            //handle load case
            loads++;
            loadBlock(cache, tag, index, wBackThru, lru_fifo, cycle_mult);
        }
        else if (op == 's') {
            //handle store case
            stores++;
            storeBlock(cache, tag, index, wAlloc, wBackThru, lru_fifo, cycle_mult);
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