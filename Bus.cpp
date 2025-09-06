#include "Bus.h"
#include <iostream>
#include <cmath> // For log2 function

Bus::Bus(std::vector<Cache*> c, Main_Memory* M) {
    this->c = c;
    block_size = c[0]->block_size;
    cache_size = c[0]->cache_size;
    this->M = M;
}

Cache_Block Bus::read_miss(const std::string& address, int proc_id) {
    std::cout << "On Bus, Read Miss at Address: " << address << ", from Processor ID: " << proc_id << std::endl;

    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);

    Cache_Block cb(block_size);
    int cnt = 0; // Stores how many caches have the particular block

    for (int i = 0; i < c.size(); i++) {
        if (i == proc_id) continue;

        if (c[i]->c_blocks[index].tag == tag) {
            // Tag matched
            if (c[i]->c_blocks[index].state == 0) {
                continue;
            } else if (c[i]->c_blocks[index].state == 1) {
                cnt++;
                cb = c[i]->c_blocks[index];
            } else {
                cnt++;
                std::string wb_addr = c[i]->c_blocks[index].tag +
                                      address.substr(address.length() - offset_size - index_size, index_size) +
                                      address.substr(address.length() - offset_size);
                write_back(wb_addr, c[i]->c_blocks[index]);
                c[i]->c_blocks[index].state = 1;
                cb = c[i]->c_blocks[index];
            }
        }
    }

    if (cnt == 0) {
        cb = M->read(address);
    }

    return cb;
}

Cache_Block Bus::write_miss(const std::string& address, int proc_id) {
    std::cout << "On Bus, Write Miss at Address: " << address << ", from Processor ID: " << proc_id << std::endl;

    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);

    Cache_Block cb(block_size);
    int cnt = 0;

    for (int i = 0; i < c.size(); i++) {
        if (i == proc_id) continue;

        if (c[i]->c_blocks[index].tag == tag) {
            if (c[i]->c_blocks[index].state == 0) {
                continue;
            } else if (c[i]->c_blocks[index].state == 1) {
                cnt++;
                cb = c[i]->c_blocks[index];
            } else {
                cnt++;
                std::string wb_addr = c[i]->c_blocks[index].tag +
                                      address.substr(address.length() - offset_size - index_size, index_size) +
                                      address.substr(address.length() - offset_size);
                write_back(wb_addr, c[i]->c_blocks[index]);
                cb = c[i]->c_blocks[index];
            }
        }
    }

    return cb;
}

void Bus::invalidate(const std::string& address, int proc_id) {
    std::cout << "Invalidation from Processor: " << proc_id << " at Address: " << address << std::endl;

    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    for (int i = 0; i < c.size(); i++) {
        if (i == proc_id) continue;
        if (c[i]->c_blocks[index].tag == tag) {
            c[i]->c_blocks[index].state = 0; // Invalidate the cache block
        }
    }
    std::cout << "Invalidation done....." << std::endl;
}

void Bus::write_back(const std::string& address, Cache_Block cb) {
    std::cout << "Write Back at Address: " << address << std::endl;
    M->write(address, cb);
}
