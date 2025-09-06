#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

#include <vector>
#include <string>
#include "Cache.h" // Include the Cache header for Cache_Block definition

class Main_Memory {
public:
    int mem_size; // in bytes
    int block_size; // in bytes
    std::vector<Cache_Block> data;

    Main_Memory(int mem_size, int block_size);

    Cache_Block read(const std::string& address);
    void write(const std::string& address, Cache_Block cb);
};

#endif
