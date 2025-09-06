#include "Processor.h"
#include <iostream>
#include <cmath> // For log2 function

Processor::Processor(Mode mode, Cache* c, Bus* bus, Directory* D, int id) {
    this->cache = c;
    block_size = cache->block_size;
    cache_size = cache->cache_size;
    this->bus = bus;
    this->dir = D;
    this->id = id;
    this->read_hits=0;
    this->read_misses=0;
    this->write_hits=0;
    this->write_misses=0;
    this->mode=mode;
}

int Processor::read(const std::string& address) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);
    if(mode == SNOOPING) {
        std::cout << "SNOOPING: Processor Read at Address: " << address << " Processor id: " << id <<std::endl;

        if (cache->c_blocks[index].state == 0) {
            // Invalid - Normal Miss
            this->read_misses++;
            cache->c_blocks[index] = bus->read_miss(address, id);
            cache->c_blocks[index].tag = tag;
            cache->c_blocks[index].state = 1;
            return cache->c_blocks[index].data[offset];
        } else if (cache->c_blocks[index].state == 1) {
            // Shared
            if (cache->c_blocks[index].tag == tag) {
                // Read Hit
                this->read_hits++;
                return cache->c_blocks[index].data[offset];
            } else {
                // Read Miss
                this->read_misses++;
                cache->c_blocks[index] = bus->read_miss(address, id);
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].state = 1;
                return cache->c_blocks[index].data[offset];
            }
        } else {
            // Modified
            if (cache->c_blocks[index].tag == tag) {
                this->read_hits++;
                return cache->c_blocks[index].data[offset];
            } else {
                this->read_misses++;
                std::string wb_addr = cache->c_blocks[index].tag +
                                    address.substr(address.length() - offset_size - index_size, index_size) +
                                    address.substr(address.length() - offset_size);
                bus->write_back(wb_addr, cache->c_blocks[index]);
                cache->c_blocks[index] = bus->read_miss(address, id);
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].state = 1;
                return cache->c_blocks[index].data[offset];
            }
        }
    } else if(mode == DIRECTORY) {
        std::cout << "DIRECTORY: Processor Read at Address: " << address << " Processor id: " << id <<std::endl;

        if(cache->c_blocks[index].state == 0) {
            //invalid
            this->read_misses++;
            cache->c_blocks[index] = dir->read_miss(address, id);
            cache->c_blocks[index].tag = tag;
            cache->c_blocks[index].state = 1;
            return cache->c_blocks[index].data[offset];
        } else if(cache->c_blocks[index].state == 1) {
            //shared
            if (cache->c_blocks[index].tag == tag) {
                // Read Hit
                this->read_hits++;
                return cache->c_blocks[index].data[offset];
            } else {
                // Read Miss
                this->read_misses++;
                cache->c_blocks[index] = dir->read_miss(address, id);
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].state = 1;
                return cache->c_blocks[index].data[offset];
            }
        } else {
            //modified
            if (cache->c_blocks[index].tag == tag) {
                this->read_hits++;
                return cache->c_blocks[index].data[offset];
            } else {
                this->read_misses++;
                std::string wb_addr = cache->c_blocks[index].tag +
                                    address.substr(address.length() - offset_size - index_size, index_size) +
                                    address.substr(address.length() - offset_size);
                dir->write_back(wb_addr, cache->c_blocks[index]);
                cache->c_blocks[index] = dir->read_miss(address, id);
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].state = 1;
                return cache->c_blocks[index].data[offset];
            }
        }
    }
    return 0;
}

void Processor::write(const std::string& address, const int data) {
    int offset, index;
    std::string tag;
    address_data ad = address_info(address, block_size, cache_size);
    offset = ad.offset;
    index = ad.index;
    tag = ad.tag;
    
    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);
    if(mode == SNOOPING) {
        std::cout << "SNOOPING: Processor Write at Address: " << address << ", data = " << data << " Processor id: " << id << std::endl;

        if (cache->c_blocks[index].state == 0) {
            // Invalid - Normal Miss
            this->write_misses++;
            cache->c_blocks[index] = bus->write_miss(address, id);
            bus->invalidate(address, id);
            cache->c_blocks[index].state = 2;
            cache->c_blocks[index].tag = tag;
            // std::cout << "HI" << std::endl;
            // std::cout << cache->c_blocks[index].data.size() << std::endl;
            cache->c_blocks[index].data[offset] = data;
        } else if (cache->c_blocks[index].state == 1) {
            // Shared
            if (cache->c_blocks[index].tag == tag) {
                this->write_hits++;
                bus->invalidate(address, id);
                cache->c_blocks[index].state = 2;
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].data[offset] = data;
            } else {
                this->write_misses++;
                cache->c_blocks[index] = bus->write_miss(address, id);
                bus->invalidate(address, id);
                cache->c_blocks[index].state = 2;
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].data[offset] = data;
            }
        } else {
            // Modified
            if (cache->c_blocks[index].tag == tag) {
                this->write_hits++;
                cache->c_blocks[index].data[offset] = data;
            } else {
                this->write_misses++;
                std::string wb_addr = cache->c_blocks[index].tag +
                                    address.substr(address.length() - offset_size - index_size, index_size) +
                                    address.substr(address.length() - offset_size);
                bus->write_back(wb_addr, cache->c_blocks[index]);
                cache->c_blocks[index] = bus->write_miss(address, id);
                bus->invalidate(address, id);
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].data[offset] = data;
            }
        }
    } else if(mode == DIRECTORY) {
        std::cout << "DIRECTORY: Processor Write at Address: " << address << ", data = " << data << " Processor id: " << id << std::endl;

        if(cache->c_blocks[index].state == 0) {
            //Invalid
            this->write_misses++;
            
            cache->c_blocks[index] = dir->write_miss(address, id);// responsiblity of directory to invalidate
            cache->c_blocks[index].state = 2;
            cache->c_blocks[index].tag = tag;
            // std::cout << "HI" << std::endl;
            // std::cout << cache->c_blocks[index].data.size() << std::endl;
            cache->c_blocks[index].data[offset] = data;
            
        } else if(cache->c_blocks[index].state == 1) {
            // Shared
            this->write_misses++;
            cache->c_blocks[index] = dir->write_miss(address, id);// responsiblity of directory to invalidate
            cache->c_blocks[index].state = 2;
            cache->c_blocks[index].tag = tag;
            cache->c_blocks[index].data[offset] = data;
        } else {
            //Modified
            if (cache->c_blocks[index].tag == tag) {
                this->write_hits++;
                cache->c_blocks[index].data[offset] = data;
            } else {
                this->write_misses++;
                std::string wb_addr = cache->c_blocks[index].tag +
                                    address.substr(address.length() - offset_size - index_size, index_size) +
                                    address.substr(address.length() - offset_size);
                dir->write_back(wb_addr, cache->c_blocks[index]);
                cache->c_blocks[index] = dir->write_miss(address, id); // responsiblity of directory to invalidate
                cache->c_blocks[index].tag = tag;
                cache->c_blocks[index].data[offset] = data;
            }
        }
    }
}
