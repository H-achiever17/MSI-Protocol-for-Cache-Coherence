#include "AddressUtils.h"
#include <stdexcept> // For exception handling

address_data address_info(const std::string& address, const int block_size, const int cache_size) {
    int offset, index;
    std::string tag;

    int offset_size = log2(block_size);
    int index_size = log2(cache_size / block_size);

    // Extract the offset from the binary address string.
    try {
        offset = std::stoi(address.substr(address.length() - offset_size), nullptr, 2);
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Offset extraction failed: address length is too short for block size");
    }

    // Extract the index from the binary address string.
    try {
        index = std::stoi(address.substr(address.length() - offset_size - index_size, index_size), nullptr, 2);
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument("Index extraction failed: address length is too short for index size");
    }

    // Extract the tag from the binary address string.
    tag = address.substr(0, address.length() - offset_size - index_size);

    address_data ad = {offset, index, tag};
    return ad;
}
