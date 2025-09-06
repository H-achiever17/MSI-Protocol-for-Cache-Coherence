#ifndef BUS_H
#define BUS_H

#include <vector>
#include <string>
#include "Cache.h"        // Include Cache definitions
#include "MainMemory.h"   // Include Main_Memory definition
#include "AddressUtils.h" // Include Address utilities

class Bus {
public:
    std::vector<Cache*> c;
    int block_size;
    int cache_size;
    Main_Memory* M;

    Bus(std::vector<Cache*> c, Main_Memory* M);

    Cache_Block read_miss(const std::string& address, int proc_id);
    Cache_Block write_miss(const std::string& address, int proc_id);
    void invalidate(const std::string& address, int proc_id);
    void write_back(const std::string& address, Cache_Block cb);
};

#endif
