//
//  cachesimulator.hpp
//  Cachesimulator
//
//  Created by Amith Kumar Yadav K on 30/10/24.
//
class Cache {
public:
    static const int mainmemory = 2048;
    static const int cachesize = 16;
    static const int slot = 20;

    static const int validbitloc = 1;    // Index in cache array for valid bit
    static const int tagloc = 2;         // Index for tag
    static const int dirtybitloc = 3;    // Index for dirty bit
    static const int dataloc = 4;        // Start index for data storage

    Cache();
    void setaddress(int addressinput);
    void calcslot();
    void calctag();
    void calcoffset();
    void setvalidbit();
    void settag();
    void setdirtybit();
    void copydata();
    void printcache();
    void readcache();
    void writecache();
    void displaycache();
    void updatemainmem();
    void calcreverseaddress();
    
    // Add these missing declarations:
    void setbeginblock();
    void setendblock();

private:
    short main_mem[mainmemory];
    int cache[cachesize][slot];
    int address, slotset, tagnumber, offset;
    int blockbegin, blockend, reverseaddress;
};

