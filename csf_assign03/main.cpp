#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <unordered_map>
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

    Slot() : tag(0), valid(false), dirty(false), load_ts(0), access_ts(0) {}
};

struct Set {
    std::vector<Slot> slots;
    std::unordered_map<uint32_t, Slot *> index;
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

std::tuple<uint32_t, uint32_t> parseAddress(uint32_t address, Cache &cache, uint index_len, uint offset_len) {
    //currentTime++; // increment time stamp
    uint32_t index;
    uint32_t tag;
    //index = (address%(2<<(offset_len + index_len - 1))) >> offset_len;
    if(index_len == 0) {
        index = 0;
    }
    else {
        index = (address<<(32 - offset_len - index_len)) >> (32 - index_len);
    }
    tag = address >> (offset_len + index_len);

    return std::make_tuple(tag, index);
}

void writeToSlot(Cache &cache, uint index, uint32_t tag, uint slot) {
    cache.sets[index].slots[slot].valid = true;
    //cache.sets[index].slots[slot].dirty = true;
    cache.sets[index].slots[slot].tag = tag;
    currentTime++;
    cache.sets[index].slots[slot].load_ts = currentTime;
    cache.sets[index].slots[slot].access_ts = currentTime;
    
}

bool writeToMap(Cache &cache, uint index, uint32_t tag, bool lru_fifo) {
    // Maps tag key to an appropriate slot (using lru or fifo)
    bool dirty = false;
    std::vector<Slot>::iterator it;
    uint32_t oldest;
    uint32_t slot = 0;
    if (lru_fifo) { // check load or access time to determine which to replace depending on lru or fifo
        oldest = cache.sets[index].slots[0].load_ts; // set oldest time to first element in slots vector
        for(it = std::next(cache.sets[index].slots.begin(),1); it != cache.sets[index].slots.end(); it++ )    {
            if(!(it->valid)) {
                slot = distance(cache.sets[index].slots.begin(), it);
                break;
            }
            if(it->load_ts < oldest) { // if any times are older, set that slot to new oldest
                oldest = it->load_ts;
                slot = distance(cache.sets[index].slots.begin(), it); // set the slot index to the index of the new oldest
            }
        }
    }
    else {
        oldest = cache.sets[index].slots[0].access_ts;
        for(it = std::next(cache.sets[index].slots.begin(),1); it != cache.sets[index].slots.end(); it++ )    {
            if(!(it->valid)) {
                slot = distance(cache.sets[index].slots.begin(), it);
                break;
            }
            if(it->access_ts < oldest) { // if any times are older, set that slot to new oldest
                oldest = it->access_ts;
                slot = distance(cache.sets[index].slots.begin(), it); // set the slot index to the index of the new oldest
            }
        }
    }
    if(cache.sets[index].slots[slot].dirty) {
        bool dirty = true;
    }
    cache.sets[index].index.erase(cache.sets[index].slots[slot].tag);
    cache.sets[index].index[tag] = &cache.sets[index].slots[slot]; // assign the tag to oldest slot
    writeToSlot(cache, index, tag, slot); // write data to slot
    return dirty;
}

void mapUpdateTs(Cache &cache, uint index, uint32_t tag) {
    currentTime++;
    cache.sets[index].index[tag]->access_ts = currentTime;
}

void updateTimestamp(Cache &cache, uint index, uint slot) {
    currentTime++;
    cache.sets[index].slots[slot].access_ts = currentTime;
}

void loadBlock(uint32_t address, Cache &cache, uint index_len, uint offset_len, bool wBackThru, bool lru_fifo) {
    uint32_t tag, index;
    std::tuple<uint32_t, uint32_t> parsedAddress;
    parsedAddress = parseAddress(address, cache, index_len, offset_len);
    tag = std::get<0>(parsedAddress);
    index = std::get<1>(parsedAddress);
    int cycle_mult = 1 << offset_len;
    cycle_mult /= 4; 
    //----------------EXPERIMENTAL-----------------
    if(cache.sets[index].index.count(tag)) { // check if this tag is in the map
        if(cache.sets[index].index[tag]->valid) { // check if slot is filled
            load_hits++;
            cycles++;
            mapUpdateTs(cache, index, tag);
        }
        else { // empty slot, load from mem
            load_misses++;
            cycles++;
            cycles+=cycle_mult*100; // load from memory cycle
            if(writeToMap(cache, index, tag, lru_fifo) && !wBackThru) { // check if dirty bit is evicted and if write-back enabled
                cycles+=cycle_mult*100; // if dirty block evicted, need to store to memory
            }
        }
    }
    else { // empty slot, load from mem
        load_misses++;
        cycles++;
        cycles+=cycle_mult*100;
        if(writeToMap(cache, index, tag, lru_fifo) && !wBackThru) { // check if dirty bit is evicted and if write-back enabled
            cycles+=cycle_mult*100; // if dirty block evicted, need to store to memory
        }
    }
    currentTime++;
    //-----------------END EXPERIMENTAL----------------

    ////-----------------------------WORKING CODE BELOW---------------------------------
    // if(cache.sets[index].slots.size() == 1) { // direct mapped case
    //     if(cache.sets[index].slots[0].valid) { // check if slot is filled
    //         if(cache.sets[index].slots[0].tag == tag) { // if the tag and index match, we load successfully
    //             load_hits++;
    //             cycles++;
    //             updateTimestamp(cache, index, 0); // update last access timestamp
    //         }
    //         else {
    //             load_misses++;
    //             cycles++;
    //             writeToSlot(cache, index, tag, 0); // overwrite old with new data
    //         }
    //     }
    //     else { // empty slot, load from mem
    //         load_misses++;
    //         cycles++;
    //         cycles+=offset_len*100;
    //         writeToSlot(cache, index, tag, 0);
    //     }
    // }
}

void storeDirtyBits(Cache &cache, uint index_len, uint offset_len) {
    std::vector<Set> sets = cache.sets;
    int dirtyCount = 0;
    for(std::vector<Set>::iterator it = sets.begin(); it != sets.end(); it++ )    {
        std::vector<Slot> slots = it->slots;
        for (std::vector<Slot>::iterator f = slots.begin(); f != slots.end(); f++) {
            if (f->dirty) {
                dirtyCount++;
            }
        } 
    }
    int cycle_mult = 1 << offset_len;
    cycle_mult /= 4; 
    cycles += cycle_mult * 100;
}
void storeBlock(uint32_t address, Cache &cache, uint index_len, uint offset_len, bool wAlloc, bool wBackThru, bool lru_fifo) {
    uint32_t tag, index;
    std::tuple<uint32_t, uint32_t> parsedAddress;
    parsedAddress = parseAddress(address, cache, index_len, offset_len);
    tag = std::get<0>(parsedAddress);
    index = std::get<1>(parsedAddress);
    int cycle_mult = 1 << offset_len;
    cycle_mult /= 4; 
//----------------EXPERIMENTAL-----------------
    if(cache.sets[index].index.count(tag) > 0) { // check if this tag is in the map
        if(cache.sets[index].index[tag]->valid) { // check if slot is filled
            store_hits++;
            mapUpdateTs(cache, index, tag);
            cycles++;
            if (wBackThru) {
                cycles+=cycle_mult*100;
            }
            else {
                // if (writeToMap(cache, index, tag, lru_fifo)) {
                //     cycles+=cycle_mult*100;
                // }
                cache.sets[index].index[tag]->dirty = true;
            }
        }
        else {
            store_misses++;
            if(wAlloc) {
                if(!wBackThru) {
                    if (writeToMap(cache, index, tag, lru_fifo)) {
                        cycles+=cycle_mult*100;
                    }
                    cache.sets[index].index[tag]->dirty = true;
                } else {
                    writeToMap(cache, index, tag, lru_fifo);
                    cycles+=cycle_mult*100;
                }
                
            } else {
               cycles+=cycle_mult*100; 
            }
            cycles++;
        }
    }
    else {
        store_misses++;
        if(wAlloc) {
            if(!wBackThru) {
                if (writeToMap(cache, index, tag, lru_fifo)) {
                    cycles+=cycle_mult*100;
                }
                cache.sets[index].index[tag]->dirty = true;
            } else {
                writeToMap(cache, index, tag, lru_fifo);
                cycles+=cycle_mult*100;
            }
        }
        else {
            cycles+=cycle_mult*100; 
        }
        cycles++;
    }
    //-----------------END EXPERIMENTAL----------------


    // //if(!wAlloc && wBackThru) {// no-write-allocate and write-through
    //     if(cache.sets[index].slots.size() == 1) { // check direct mapped case
    //         if(cache.sets[index].slots[0].valid) { // check if slot is filled (valid = true)
    //             if(cache.sets[index].slots[0].tag == tag) { // check if tags are equal
    //                 updateTimestamp(cache, index, 0);
    //                 store_hits++;
    //                 cycles++;
    //                 cycles+=offset_len*100;
    //             }
    //             else {
    //                 store_misses++;
    //                 cycles++;
    //             }
    //         }
    //         else { // for no-write-allocate do nothing
    //             store_misses++;
    //             cycles++;
    //         }
    //     }
    // //}
}


//block offset is memory address mod 2n (offset based on size of blocks in the memory (length of an element stored))
int main (int argc, char *argv[])  {
    std::string arg4(argv[4]);
    std::string arg5(argv[5]);
    std::string arg6(argv[6]);
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
    }
    else if (arg5 == "write-back" && arg4 == "no-write-allocate") {
            std::cerr << "write-back and no-write-allocate cannot be used at the same time\n";
            return 1;
    }
    bool wAlloc; // false is no-write-allocate, true is write-allocate
    bool wBackThru; // false is write-back, true is write-through
    bool lru_fifo; // false is lru, true is fifo

    // store cache configuration options to booleans
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
    uint tag_len = 32 - offset_len - index_len; // compute tag length by subtracting offset and index length from the length of the memory address
    std::vector<Set> sets(num_sets);


    std::vector<Set>::iterator it;

    Slot blank_slot;
    blank_slot.valid = false;
    blank_slot.dirty = false;
    for(it = sets.begin(); it != sets.end(); it++ )    {
        std::vector<Slot> slots(num_blocks);
        it->slots = slots;
    }


    Cache cache;
    cache.sets = sets;

    char op; // operation (load or store)
    std::string address; // memory address
    std::string dummy; // dummy variable to store third argument, necessary for iterating through cin

    // Read through the trace file line by line
    while(std::cin >> op) { // store l or s in op
        std::cin >> address; // store memory address
        std::cin >> dummy; // store third number in dummy buffer
        currentTime++; // increment global time
        if (op == 'l') {
            loadBlock(stoul(address.substr(2),nullptr, 16), cache, index_len, offset_len, wBackThru, lru_fifo);
            loads++;
        }
        else if (op == 's') {
            storeBlock(stoul(address.substr(2), nullptr, 16), cache, index_len, offset_len, wAlloc, wBackThru, lru_fifo);
            stores++;
        }
    }
    storeDirtyBits(cache, index_len, offset_len);
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