// #include <bits/stdc++.h>
// using namespace std;

// // Considering Address = Tag + Index + Offset
// // Offset = log2(block size)
// // Index = log2(cache_size/block_size)
// // Tag = log2(mm_size)(generally 32) - Offset - Index = log2(1 cache block corres to how many MM blocks)
// struct address_data{
//     int offset;
//     int index;
//     string tag;
// };

// address_data address_info(const string address, const int block_size, const int cache_size) {
//     int offset, index;
//     string tag;
//     int offset_size = log2(block_size);
//     int index_size = log2(cache_size/block_size);
//     // supposed to find integer values, check if actually works or not(slight danger :) )
//     offset = stoi(address.substr(address.length()-offset_size, address.length()), nullptr, 2);
//     index = stoi(address.substr(address.length()-offset_size-index_size, address.length()-offset_size), nullptr, 2);
//     tag = address.substr(0, address.length()-offset_size-index_size);
//     address_data ad = {offset, index, tag};
//     return ad;
// }

// class Cache_Block {
//     public:
//     vector<int> data;
//     // per index = 1 byte(considered)
//     int state;
//     /* 0 - Invalid
//        1 - Shared
//        2 - Modified/Exclusive*/
//     string tag; 
//     // unique idntifier for the same offset and same index
// };

// class Cache {
//     public:
//     vector<Cache_Block> c_blocks; 
//     int cache_size;
//     int block_size;
//     int id; // processor id
//     Cache(const unsigned int cache_size, const unsigned int block_size,const int id) {
//         /* cache size in bytes, 
//            block size in bytes*/
//         this->c_blocks.assign(cache_size/block_size, {vector<int>(block_size, 0), 0, "x"});
//         this->cache_size = cache_size;
//         this->block_size = block_size;
//         this->id = id;
//     }
// };

// class Processor {
//     public:
//     int id;
//     Cache* cache;
//     int block_size;
//     int cache_size;
//     Bus* bus;
//     Processor(Cache* c, Bus* bus, int id) {
//         this->cache = c;
//         block_size = cache->block_size;
//         cache_size = cache->cache_size;
//         this->bus = bus;
//         this->id = id;
//     }
//     int read(const string address) {
//         cout << "Processor Read at Address: " << address << endl;
//         int offset, index;
//         string tag;
//         address_data ad = address_info(address, block_size, cache_size);
//         offset = ad.offset;index = ad.index;tag = ad.tag;
//         int offset_size = log2(block_size);
//         int index_size = log2(cache_size/block_size);
//         if(cache->c_blocks[index].state == 0) {
//             //invalid
//             //Normal Miss
//             //put read miss on bus, will recieve the block
//             //put this block to shared and fill the tag
//             //return the required data to processor
//             cache->c_blocks[index] = bus->read_miss(address, id); // read miss will return the block somehow, either from other cache or from Main Mem.
//             cache->c_blocks[index].tag = tag;
//             cache->c_blocks[index].state = 1;
//             return cache->c_blocks[index].data[offset];

//         } else if(cache->c_blocks[index].state == 1) {
//             //shared
//             if(cache->c_blocks[index].tag == tag) {
//                 //Read hit
//                 return cache->c_blocks[index].data[offset];
//             } else {
//                 //Read Miss
//                 cache->c_blocks[index] = bus->read_miss(address, id);
//                 cache->c_blocks[index].tag = tag;
//                 cache->c_blocks[index].state = 1;
//                 return cache->c_blocks[index].data[offset];
//             }
//         } else {
//             //modified
//             if(cache->c_blocks[index].tag == tag) {
//                 return cache->c_blocks[index].data[offset];
//             } else {
//                 string wb_addr = cache->c_blocks[index].tag+address.substr(address.length()-offset_size-index_size, address.length()-offset_size)+address.substr(address.length()-offset_size, address.length());
//                 bus->write_back(wb_addr, cache->c_blocks[index]);
//                 cache->c_blocks[index] = bus->read_miss(address, id);
//                 cache->c_blocks[index].tag = tag;
//                 cache->c_blocks[index].state = 1;
//                 return cache->c_blocks[index].data[offset];
//             }
//         }
//         return 0;
//     }
//     void write(const string address, const int data) {
//         cout << "Processor Write at Address: " << address << ", data = " << data <<endl;

//         int offset, index;
//         string tag;
//         address_data ad = address_info(address, block_size, cache_size);
//         offset = ad.offset;index = ad.index;tag = ad.tag;
//         int offset_size = log2(block_size);
//         int index_size = log2(cache_size/block_size);

//         if(cache->c_blocks[index].state == 0) {
//             //invalid
//             //Normal Miss
//             cache->c_blocks[index] = bus->write_miss(address, id); // write miss will return the block somehow, either from other cache or from Main Mem.
//             bus->invalidate(address, id);
//             cache->c_blocks[index].state = 2;
//             cache->c_blocks[index].tag = tag;
//             cache->c_blocks[index].data[offset] = data; 
//         } else if(cache->c_blocks[index].state == 1) {
//             //shared
//             if(cache->c_blocks[index].tag == tag) {
//                 bus->invalidate(address, id);
//                 cache->c_blocks[index].state = 2;
//                 cache->c_blocks[index].tag = tag;
//                 cache->c_blocks[index].data[offset] = data;
//             } else {
//                 cache->c_blocks[index] = bus->write_miss(address, id);
//                 bus->invalidate(address, id);
//                 cache->c_blocks[index].state = 2;
//                 cache->c_blocks[index].tag = tag;
//                 cache->c_blocks[index].data[offset] = data;
//             }
//         } else {
//             //modified
//             if(cache->c_blocks[index].tag == tag) {
//                 cache->c_blocks[index].data[offset] = data;
//             } else {
//                 string wb_addr = cache->c_blocks[index].tag+address.substr(address.length()-offset_size-index_size, address.length()-offset_size)+address.substr(address.length()-offset_size, address.length());
//                 bus->write_back(wb_addr, cache->c_blocks[index]);
//                 cache->c_blocks[index] = bus->write_miss(address, id);
//                 bus->invalidate(address, id);
//                 cache->c_blocks[index].tag = tag;
//                 cache->c_blocks[index].data[offset] = data;
//             }
//         }
//     }
    
// };

// class Bus {
//     public:
//     vector<Cache*> c;
//     int block_size;
//     int cache_size;
//     Main_Memory* M;
//     Bus(vector<Cache*> c, Main_Memory* M) {
//         this->c = c;
//         block_size = c[0]->block_size;
//         cache_size = c[0]->cache_size;
//         this->M = M;
//     }
//     Cache_Block read_miss(const string address, int proc_id) {
//         cout << "On Bus, Read Miss at Address: " << address << ", from Processor ID: " << proc_id <<  endl;

//         int offset, index;
//         string tag;
//         address_data ad = address_info(address, block_size, cache_size);
//         offset = ad.offset;index = ad.index;tag = ad.tag;
//         int offset_size = log2(block_size);
//         int index_size = log2(cache_size/block_size);

//         Cache_Block cb;
//         int cnt = 0; // stores in how many caches,  the particular 
//         for(int i=0; i<c.size(); i++) {
//             if(i == proc_id) continue;
//             if(c[i]->c_blocks[index].tag == tag) {
//                 // tag matched
//                 if(c[i]->c_blocks[index].state == 0) {
//                     continue;
//                 } else if(c[i]->c_blocks[index].state == 1) {
//                     cnt++;
//                     cb = c[i]->c_blocks[index];
//                 } else {
//                     cnt++;
//                     string wb_addr = c[i]->c_blocks[index].tag+address.substr(address.length()-offset_size-index_size, address.length()-offset_size)+address.substr(address.length()-offset_size, address.length());
//                     write_back(wb_addr, c[i]->c_blocks[index]);
//                     c[i]->c_blocks[index].state = 1;
//                     cb = c[i]->c_blocks[index];
//                 }
//             } else {
//                 continue;
//             }
//         }
//         if(cnt == 0) cb = M->read(address);
//         return cb;
//     }
//     Cache_Block write_miss(const string address, int proc_id) {
//         cout << "On Bus, Write Miss at Address: " << address << ", from Processor ID: " << proc_id << endl;
//         int offset, index;
//         string tag;
//         address_data ad = address_info(address, block_size, cache_size);
//         offset = ad.offset;index = ad.index;tag = ad.tag;
//         int offset_size = log2(block_size);
//         int index_size = log2(cache_size/block_size);

//         Cache_Block cb;
//         int cnt=0;
//         for(int i=0; i<c.size(); i++) {
//             if(i == proc_id) continue;
//             if(c[i]->c_blocks[index].tag == tag) {
//                 if(c[i]->c_blocks[index].state == 0) {
//                     continue;
//                 } else if(c[i]->c_blocks[index].state == 1) {
//                     cnt++;
//                     cb = c[i]->c_blocks[index];
//                 } else {
//                     cnt++;
//                     string wb_addr = c[i]->c_blocks[index].tag+address.substr(address.length()-offset_size-index_size, address.length()-offset_size)+address.substr(address.length()-offset_size, address.length());
//                     write_back(wb_addr, c[i]->c_blocks[index]);
//                     cb = c[i]->c_blocks[index];
//                 }
//             } else {
//                 continue;
//             }
//         }
//         return cb;
//     }
//     void invalidate(const string address, int proc_id) {
//         cout << "Invalidation from Processor: " << proc_id << " at Address: " << address << endl;
//         int offset, index;
//         string tag;
//         address_data ad = address_info(address, block_size, cache_size);
//         offset = ad.offset;index = ad.index;tag = ad.tag;
//         for(int i=0; i<c.size(); i++) {
//             if(i == proc_id) continue;
//             c[i]->c_blocks[index].state = 0;
//         }
//     }
//     void write_back(const string address, Cache_Block cb) {
//         cout << " Write Back at Address: " << address << endl;
//         M->write(address, cb);
//     }
// };

// class Main_Memory {
//     public:
//     int mem_size;// in bytes
//     int block_size; // in bytes
//     vector<Cache_Block> data;
//     Main_Memory(int mem_size, int block_size) {
//         this->data.resize(mem_size/block_size);
//         this->mem_size = mem_size;
//         this->block_size = block_size;
//     }
//     Cache_Block read(const string address) {
//         int address_pos = stoi(address, nullptr, 2);
//         int offset_size = log2(block_size);
//         address_pos = address_pos>>offset_size;
//         return data[address_pos];
//     }
//     void write(const string address, Cache_Block cb) {
//         int address_pos = stoi(address, nullptr, 2);
//         int offset_size = log2(block_size);
//         address_pos = address_pos>>offset_size;
//         data[address_pos] = cb;
//     }
// };

#include <iostream>
#include <vector>
#include "Cache.h"       // Include Cache class
#include "Processor.h"   // Include Processor class
#include "Bus.h"         // Include Bus class
#include "MainMemory.h"  // Include Main_Memory class
#include "Directory.h"

using namespace std;

int main() {
    int num_proc=2;
    const unsigned int block_size = 4;//2^2
    const unsigned int total_mem_size = 1024;//2^10
    const unsigned int cache_size = 32;  // 2^5
    Mode mode = SNOOPING; // DIRECTORY
    Main_Memory* M = new Main_Memory(total_mem_size, block_size);
    //int num_proc = 8;
    vector<Processor*> processors(num_proc);
    vector<Cache*> caches(num_proc);
    for(int i=0; i<num_proc; i++){
        caches[i] = new Cache(cache_size, block_size, i);
    }
    Bus* B = new Bus(caches, M);
    Directory* D = new Directory(M, caches);
    for(int i=0; i<num_proc; i++) {
        processors[i] = new Processor(mode, caches[i], B, D, i);
    }
    // test bench program
    vector<string> prog = {
        "Pr-1 Wr 0101010110 4",
        "Pr-0 Rd 0101010110",
        "Pr-1 Rd 0101010110",
        "Pr-0 Wr 0101010110 7",
        "Pr-0 Rd 0101010110",
        "Pr-1 Rd 0101010110",
    };

    for(int i=0; i<prog.size(); i++) {
        size_t space1 = prog[i].find(' ');
        size_t space2 = prog[i].find(' ', space1 + 1);
        size_t space3 = prog[i].find(' ', space2 + 1);
        size_t space4 = prog[i].find(' ', space3 + 1);
        
        int proc_no = stoi(prog[i].substr(3, space1 - 3), nullptr, 2);

        string instr_type = prog[i].substr(space1 + 1, space2 - space1 - 1);
        
        string address;
        if (space3 != string::npos) { 
            address = prog[i].substr(space2 + 1, space3 - space2 - 1);
        } else { 
            address = prog[i].substr(space2 + 1);
        }
        //cout << proc_no << " " << instr_type << " " << address << endl;
        if(instr_type == "Rd") {
            processors[proc_no]->read(address);
        } else if(instr_type == "Wr") {
            
            int value;
            if (space3 != std::string::npos && space4 == std::string::npos) {
                
                value = stoi(prog[i].substr(space3 + 1), nullptr, 10); // The rest of the string is the value
            }
            //cout << "VALUE: " << value << endl;
            processors[proc_no]->write(address, value);
        }
        cout << "INSTRUCTION " << i << " COMPLELETED" << endl;
    }
    cout << "*********************************************" << endl;
    cout << "Performance:\n" << endl;
    for(int i=0; i<num_proc; i++) {
        cout << "Processor id: " << i << endl;
        cout << "Read Hit Rate: " << ((double)processors[i]->read_hits)/(processors[i]->read_hits + processors[i]->read_misses) << endl;
        cout << "Read Miss Rate: " << ((double)processors[i]->read_misses)/(processors[i]->read_hits + processors[i]->read_misses) << endl;
        cout << "Write Hit Rate: " << ((double)processors[i]->write_hits)/(processors[i]->write_hits + processors[i]->write_misses) << endl;
        cout << "Write Miss Rate: " << ((double)processors[i]->write_misses)/( processors[i]->write_hits + processors[i]->write_misses) << endl;
        cout << "----------------------------------------------" << endl;
    }

    delete M;
    delete B;
    delete D;
    for (int i = 0; i < num_proc; i++) {
        delete processors[i];
        delete caches[i];
    }
    return 0;
}


// int main() {
//     // Define system parameters
//     int num_proc = 2; // Number of processors
//     const unsigned int block_size = 4;       // Block size in bytes (2^2 = 4 bytes)
//     const unsigned int total_mem_size = 1024; // Total main memory size in bytes
//     const unsigned int cache_size = 256;      // Cache size per processor in bytes

//     // Create Main Memory
//     Main_Memory* M = new Main_Memory(total_mem_size, block_size);

//     // Create caches for each processor
//     vector<Cache*> caches(num_proc);
//     for (int i = 0; i < num_proc; i++) {
//         caches[i] = new Cache(cache_size, block_size, i);
//     }

//     // Create shared bus with the caches and main memory
//     Bus* B = new Bus(caches, M);

//     // Create processors, each with its cache and access to the bus
//     vector<Processor*> processors(num_proc);
//     for (int i = 0; i < num_proc; i++) {
//         processors[i] = new Processor(caches[i], B, i);
//     }

//     // Test Bench Program to simulate processor operations
//     //cout << "Processor 1 writes 4 to address '0101010110'" << endl;
//     processors[1]->write("0101010110", 4);

//     //cout << "Processor 0 reads from address '0101010110': " << endl;
//     cout << "Data read by Processor 0: " << processors[0]->read("0101010110") << endl;

//     cout << "Processor 1 reads from address '0101010110': " << endl;
//     cout << "Data read by Processor 1: " << processors[1]->read("0101010110") << endl;

//     cout << "Processor 0 writes 7 to address '0101010110'" << endl;
//     processors[0]->write("0101010110", 7);

//     cout << "Processor 0 reads from address '0101010110': " << endl;
//     cout << "Data read by Processor 0: " << processors[0]->read("0101010110") << endl;

//     cout << "Processor 1 reads from address '0101010110': " << endl;
//     cout << "Data read by Processor 1: " << processors[1]->read("0101010110") << endl;

//     // Cleanup dynamically allocated memory
//     delete M;
//     delete B;
//     for (int i = 0; i < num_proc; i++) {
//         delete processors[i];
//         delete caches[i];
//     }

//     return 0;
// }
