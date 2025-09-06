#include "Cache.h"
Cache_Block::Cache_Block(int block_size) { 
    this->data = std::vector<int>(block_size, 0); 
    this->state=0; 
    this->tag="x";
}
Cache::Cache(const unsigned int cache_size, const unsigned int block_size, const int id) {
    this->c_blocks.assign(cache_size / block_size, Cache_Block(block_size));
    this->cache_size = cache_size;
    this->block_size = block_size;
    this->id = id;
}
