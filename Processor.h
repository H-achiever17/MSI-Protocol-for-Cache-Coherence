#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include "Cache.h"       // Include Cache header file
#include "Bus.h"         // Include Bus header file
#include "Directory.h"    // Include Directory header file
#include "AddressUtils.h" // Include Address utilities
enum Mode {
    SNOOPING, 
    DIRECTORY
};

class Processor {
public:
    
    int id;
    Cache* cache;
    int block_size;
    int cache_size;
    Bus* bus;
    Directory* dir;
    int read_hits;
    int write_hits;
    int read_misses;
    int write_misses;
    Mode mode;

    Processor(Mode mode, Cache* c, Bus* bus, Directory* D, int id);

    int read(const std::string& address);
    void write(const std::string& address, const int data);
};

#endif
