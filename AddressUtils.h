#ifndef ADDRESS_UTILS_H
#define ADDRESS_UTILS_H

#include <string>
#include <cmath> // To use log2

struct address_data {
    int offset;
    int index;
    std::string tag;
};

/**
 * @brief Extracts the offset, index, and tag from a given address.
 * 
 * @param address The address in binary string format.
 * @param block_size The size of each block in the cache.
 * @param cache_size The total size of the cache.
 * @return address_data A struct containing the offset, index, and tag.
 */
address_data address_info(const std::string& address, const int block_size, const int cache_size);

#endif
