#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Cache.h"
#include "MainMemory.h"
#include <vector>
#include <string>
#include <set>
enum STATE_MM {
    UNCACHED,
    SHARED,
    EXCLUSIVE
};
class Directory {
    public:
    int num_procs;
    int num_blocks_MM;
    int block_size;
    int cache_size;
    Main_Memory* M;
    std::vector<std::vector<int>> p;
    std::vector<std::set<int>> sharers;
    std::vector<STATE_MM> state;
    std::vector<int> dirty;
    std::vector<Cache*> c;

    Directory(Main_Memory* M, std::vector<Cache*> c);

    Cache_Block read_miss(const std::string& address, int proc_id);
    Cache_Block write_miss(const std::string& address, int proc_id);
    void invalidate(const std::string& address, int proc_id);
    void write_back(const std::string& address, Cache_Block cb);

};

#endif