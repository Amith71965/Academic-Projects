//
//  main.cpp
//  Pipeline
//
//  Created by Amith Kumar Yadav K
//

#include "Pipeline.hpp"
#include <iostream>
using namespace std;

int main() {
    Pipeline pipeline;
    
    // Test program instructions as given in the assignment
    unsigned int instructions[] = {  // Changed from int to unsigned int
        0xa1020000u,   // sb $2, 0($8)  Added 'u' suffix to mark as unsigned
        0x810AFFFCu,   // lb $10, -4($8)  This was necessary to include, that is putting suffix 'u' cox I                  was getting error before without 'u' or Unsigned int
        0x00831820u, // add $3, $4, $3
        0x01263820u, // add $7, $9, $6
        0x01224820u, // add $9, $9, $2
        0x81180000u, // lb $24, 0($8)
        0x81510010u, // lb $17, 16($10)
        0x00624022u, // sub $8, $3, $2
        0x00000000u, // nop
        0x00000000u, // nop
        0x00000000u, // nop
        0x00000000u // nop
    };
    
    // Number of cycles needed is 12 based on Assignment Instruction
    const int numCycles = sizeof(instructions)/sizeof(instructions[0]);
    int pc = 0;  // Program counter
    
    // Execute pipeline for each cycle
    for(int cycle = 1; cycle <= numCycles; cycle++) {
        cout << "\n\nClock Cycle " << cycle << endl;
        cout << "----------------" << endl;
        
        // Execute pipeline stages as specified in the assignment requirement and are in the Ordered manner
        if(pc < static_cast<int>(sizeof(instructions)/sizeof(instructions[0]))) {
            pipeline.IF_stage(static_cast<int>(instructions[pc]));
            pc++;
        } else {
            pipeline.IF_stage(0);  // NOP
        }
        
        pipeline.ID_stage();
        pipeline.EX_stage();
        pipeline.MEM_stage();
        pipeline.WB_stage();
        
        // Print state of pipeline registers and CPU registers
        pipeline.Print_out_everything();
        
        // Copy write pipeline registers to read pipeline registers
        pipeline.Copy_write_to_read();
    }
    
    return 0;
}
