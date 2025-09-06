#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <string>

class Cache_Block {
public:
    std::vector<int> data; // Per index = 1 byte (considered)
    int state;             // 0 - Invalid, 1 - Shared, 2 - Modified/Exclusive
    std::string tag;       // Unique identifier for the same offset and index
    Cache_Block() {}
    Cache_Block(int block_size); 
};

class Cache {
public:
    std::vector<Cache_Block> c_blocks;
    int cache_size;
    int block_size;
    int id; // Processor ID

    Cache(const unsigned int cache_size, const unsigned int block_size, const int id);
};

#endif
