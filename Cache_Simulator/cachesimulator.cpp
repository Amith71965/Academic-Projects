//
//  cachesimulator.cpp
//  Cachesimulator
//
//  Created by Amith Kumar Yadav K on 30/10/24.
//

#include <iostream>
#include <iomanip>
#include "cachesimulator.hpp"

using namespace std;

// Constructor to initialize main memory and cache
Cache::Cache(){
    int count = 0;
    while(count < mainmemory) {
        for(int i = 0; i < 0xFF + 1; i++) {
            main_mem[count] = i;
            count++;
        }
    }
    
    int countslot = 0;
    for (size_t i = 0; i < cachesize; i++) {
        cache[i][0] = countslot;
        countslot++;
        for(size_t j = 1; j < slot; j++) {
            cache[i][j] = 0;
        }
    }
}

// Calculate address
void Cache::setaddress(int addressinput) {
    address = addressinput;
}

// Calculate slot
void Cache::calcslot() {
    slotset = (address & 0x0F0) >> 4;
}

// Calculate tag
void Cache::calctag() {
    tagnumber = address >> 8;
}

// Set offset
void Cache::calcoffset() {
    offset = address & 0x00F;
}

// Set valid bit to 1
void Cache::setvalidbit() {
    cache[slotset][validbitloc] = 1;
}

// Set tag in cache
void Cache::settag() {
    cache[slotset][tagloc] = tagnumber;
}

// Calculate beginning of block
void Cache::setbeginblock() {
    blockbegin = address & 0xFF0;
}

// Calculate end of block
void Cache::setendblock() {
    blockend = blockbegin + 0x00F;
}

// Set dirty bit
void Cache::setdirtybit() {
    if(cache[slotset][dirtybitloc] == 1) {
        cache[slotset][dirtybitloc] = 0;
    } else {
        cache[slotset][dirtybitloc] = 1;
    }
}

// Copy data from main memory to cache
void Cache::copydata() {
    setbeginblock();
    setendblock();
    int count2 = 0;
    for(int j = dataloc; j < slot; j++) {
        cache[slotset][j] = main_mem[blockbegin + count2];
        count2++;
    }
}

// Print cache line data for a given slotset
void Cache::printcache() {
    for(size_t i = 0; i < slot; i++) {
        cout << hex << setw(2) << setfill('0') << cache[slotset][i] << " ";
    }
    cout << endl;
}

// Read from cache
void Cache::readcache() {
    //cout << "(R)ead, (W)rite, (D)isplay cache, (Q)uit: ";
    //char option;
    //cin >> option;
    //if(option != 'R' && option != 'r') return;
    
    cout << "What address would you like to read? ";
    short addressinput;
    cin >> hex >> addressinput;
    if(addressinput > 0x7FF) {
        cout << "Address out of bounds. Try a smaller address." << endl;
        return;
    }

    setaddress(addressinput);
    calcslot();
    calctag();
    calcoffset();

    if(cache[slotset][tagloc] == tagnumber && cache[slotset][validbitloc] == 1) {
        cout << "At address " << hex << address << ", there is the value " << hex << cache[slotset][dataloc + offset] << " (Cache Hit)" << endl;
    } else {
        cout << "CACHE MISS" << endl;
        copydata();
        setvalidbit();
        settag();
        cout << "Now in cache: ";
        printcache();
        cout << "At address " << hex << address << ", there is the value " << hex << cache[slotset][dataloc + offset] << endl;
    }
}

// Write to cache
void Cache::writecache() {
    //cout << "(R)ead, (W)rite, or (D)isplay Cache? ";
    //char option;
    //cin >> option;
    //if(option != 'W' && option != 'w') return;
    
    cout << "What address would you like to write to? ";
    int addressinput;
    cin >> hex >> addressinput;
    if(addressinput > 0x7FF) {
        cout << "Address out of bounds. Try a smaller address." << endl;
        return;
    }

    setaddress(addressinput);
    calcslot();
    calctag();
    calcoffset();

    cout << "What data would you like to write at that address? ";
    int datainput;
    cin >> hex >> datainput;

    if(cache[slotset][tagloc] == tagnumber && cache[slotset][validbitloc] == 1) {
        cout << "CACHE HIT" << endl;
        cache[slotset][dataloc + offset] = datainput;
        setdirtybit();
        cout << "Value " << hex << datainput << " has been written to address " << hex << addressinput << " (Cache Hit)" << endl;
    } else {
        cout << "CACHE MISS" << endl;
        copydata();
        setvalidbit();
        settag();
        cache[slotset][dataloc + offset] = datainput;
        setdirtybit();
        cout << "Value " << hex << datainput << " has been written to address " << hex << addressinput << " (Cache Miss)" << endl;
    }
}

// Display the full cache contents
void Cache::displaycache() {
    //cout << "\n(R)ead, (W)rite, or (D)isplay Cache? ";
    //char option;
    //cin >> option;
    //if(option != 'D' && option != 'd') return;
    
    cout << "Slot Valid Tag Dirty Data\n";
    for(size_t i = 0; i < cachesize; i++) {
        cout << setw(2) << i << "    " << cache[i][validbitloc] << "    " << setw(2) << cache[i][tagloc] << "   " << cache[i][dirtybitloc] << "    ";
        for(size_t j = dataloc; j < slot; j++) {
            cout << hex << setw(2) << setfill('0') << cache[i][j] << " ";
        }
        cout << endl;
    }
}

void Cache::updatemainmem() {
    calcreverseaddress();
    int count3 = 0;
    for(int i = dataloc; i < slot; i++) {
        main_mem[reverseaddress + count3] = cache[slotset][i];
        count3++;
    }
}

void Cache::calcreverseaddress() {
    reverseaddress = (tagnumber << 8) + (slotset << 4);
}
