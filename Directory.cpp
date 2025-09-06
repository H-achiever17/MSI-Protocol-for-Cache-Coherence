#include "Directory.h"
#include "AddressUtils.h"
#include <iostream>

Directory::Directory(Main_Memory* M, std::vector<Cache*> c) {
    this->M = M;
    this->c = c;
    this->num_blocks_MM = M->mem_size/M->block_size;
    this->block_size = c[0]->block_size;
    this->cache_size = c[0]->cache_size;
    this->num_procs = c.size();
    this->dirty.resize(this->num_blocks_MM);
    this->sharers.resize(num_blocks_MM);
    (this->p).assign(num_blocks_MM, std::vector<int>(num_procs, 0));
    (this->state).assign(num_blocks_MM, UNCACHED);
}

Cache_Block Directory::read_miss(const std::string& address, int proc_id) {
    std::cout << "Directory: Read Miss at Address: " << address << ", from Processor ID: " << proc_id << std::endl;

    Cache_Block cb(block_size);
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);
    int address_pos = std::stoi(address, nullptr, 2);
    address_pos = address_pos >> offset_size;


    if(state[address_pos] == UNCACHED || state[address_pos] == SHARED) {
        cb = M->read(address);
        sharers[address_pos].insert(proc_id);
        p[address_pos][proc_id] = 1;
        state[address_pos] = SHARED;
    } else if(state[address_pos] == EXCLUSIVE){
        int dirty_proc_id = *sharers[address_pos].begin();
        cb = c[dirty_proc_id]->c_blocks[index];
        sharers[address_pos].insert(proc_id);
        write_back(address, cb);
        p[address_pos][proc_id] = 1;
        state[address_pos] = SHARED;
        dirty[address_pos] = 0;
    }
    return cb;
}
Cache_Block Directory:: write_miss(const std::string& address, int proc_id) {
    std::cout << "DIRECTORY: Write Miss at Address: " << address << ", from Processor ID: " << proc_id << std::endl;

    Cache_Block cb(block_size);
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);
    int address_pos = std::stoi(address, nullptr, 2);
    address_pos = address_pos >> offset_size;
    
    if(state[address_pos] == UNCACHED || state[address_pos] == SHARED) {
        cb = M->read(address);
        invalidate(address, proc_id);
        sharers[address_pos].insert(proc_id);
        p[address_pos][proc_id] = 1;
        dirty[address_pos] = 1;
        state[address_pos] = EXCLUSIVE;
        //std::cout << "hello" << std::endl;
    } else if(state[address_pos] == EXCLUSIVE) {
        int dirty_proc_id = *sharers[address_pos].begin();
        cb = c[dirty_proc_id]->c_blocks[index];
        invalidate(address, proc_id);
        sharers[address_pos].insert(proc_id);
        write_back(address, cb);
        p[address_pos][proc_id] = 1;
        state[address_pos] = EXCLUSIVE;
        dirty[address_pos] = 1;
    }
    return cb;
}
void Directory::invalidate(const std::string& address, int proc_id) {
    std::cout << "DIRECTORY: Invalidation For Address: " << address << ", from Processor ID: " << proc_id << std::endl;

    Cache_Block cb(block_size);
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);
    int address_pos = std::stoi(address, nullptr, 2);
    address_pos = address_pos >> offset_size;

    //DEFINE LOGIC LEFT......
    while(!sharers[address_pos].empty()) {
        int pid = *sharers[address_pos].begin();
        c[pid]->c_blocks[index].state = 0;
        sharers[address_pos].erase(pid);
        p[address_pos][pid] = 0;

    }
    std::cout << "Invalidation Done....." << std::endl; 
    state[address_pos] = UNCACHED;
    dirty[address_pos] = 0;

}
void Directory::write_back(const std::string& address, Cache_Block cb) {
    std::cout << "Directory: Write Back for Address: " << address << std::endl;
    M->write(address, cb);
}
