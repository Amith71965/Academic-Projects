//
//  main.cpp
//  Cachesimulator
//
//  Created by Amith Kumar Yadav K on 30/10/24.
//

#include <iostream>
#include "cachesimulator.hpp"
using namespace std;

int main() {
    Cache myCache;

    char option;
    while (true) {
        cout << "(R)ead, (W)rite, (D)isplay cache, (Q)uit: ";
        cin >> option;

        if (option == 'R' || option == 'r') {
            myCache.readcache();
        } else if (option == 'W' || option == 'w') {
            myCache.writecache();
        } else if (option == 'D' || option == 'd') {
            myCache.displaycache();
        } else if (option == 'Q' || option == 'q') {
            break;
        } else {
            cout << "Invalid option, try again.\n";
        }
    }
    return 0;
}
