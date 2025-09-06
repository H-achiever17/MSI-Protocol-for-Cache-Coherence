#include "MainMemory.h"
#include <cmath>    // For log2 function
#include <iostream> // Optional: For debugging messages if needed

Main_Memory::Main_Memory(int mem_size, int block_size) {
    this->data.resize(mem_size / block_size);
    for(int i=0; i<mem_size/block_size; i++) {
        this->data[i] = Cache_Block(block_size);
    }
    this->mem_size = mem_size;
    this->block_size = block_size;
}

Cache_Block Main_Memory::read(const std::string& address) {
    int address_pos = std::stoi(address, nullptr, 2);
    int offset_size = log2(block_size);
    address_pos = address_pos >> offset_size;

    if (address_pos < 0 || address_pos >= data.size()) {
        throw std::out_of_range("Read address out of range");
    }

    return data[address_pos];
}

void Main_Memory::write(const std::string& address, Cache_Block cb) {
    int address_pos = std::stoi(address, nullptr, 2);
    int offset_size = log2(block_size);
    address_pos = address_pos >> offset_size;

    if (address_pos < 0 || address_pos >= data.size()) {
        throw std::out_of_range("Write address out of range");
    }

    data[address_pos] = cb;
}
