//
//  Pipeline.cpp
//  Pipeline
//
//  Created by Amith Kumar Yadav K
//

#include "Pipeline.hpp"
#include <iostream>
using namespace std;

Pipeline::Pipeline() {
    // Initialize Main Memory
    int count = 0;
    while(count < mainmemory) {
        for(short i = 0; i < 0xFF + 1; i++) {
            Main_Mem[count] = i;
            count++;
        }
    }
    
    // Initialize Registers
    for(int i = 1; i < regsNum; i++) {
        Regs[0] = 0;
        Regs[i] = 0x100 + i;
    }
}

void Pipeline::IF_stage(unsigned int instruction) {
    ifidwrite.setinst(instruction);
}

void Pipeline::ID_stage() {
    unsigned int instruction = ifidread.getinst();
    
    if(instruction != 0) {
        // Calculate opcode
        int opcode = (instruction & 0xFC000000u) >> 26;
        
        // Get register numbers
        int rs = (instruction & 0x03E00000u) >> 21;
        idexwrite.setreadreg1(Regs[rs]);
        
        int rt = (instruction & 0x001F0000u) >> 16;
        idexwrite.setreadreg2(Regs[rt]);
        
        int rd = (instruction & 0x0000F800u) >> 11;
        
        // Set write registers
        idexwrite.setwritereg1(rt);  // For lb type instruction
        idexwrite.setwritereg2(rd);  // For R type instruction
        
        // Calculate offset
        int Offset = (ifidread.getinst() & 0x0000FFFF);
        idexwrite.setoffset(Offset);
        
        // Get function code
        int function = (ifidread.getinst() & 0x0000003F);
        idexwrite.setfunction(function);
        
        // Set control signals based on instruction type
        if(opcode == 0)
        {  // R-type
            idexwrite.setregdst(1);
            idexwrite.setalusrc(0);
            idexwrite.setaluop(10);
            idexwrite.setmemread(0);
            idexwrite.setmemwrite(0);
            idexwrite.setmemtoreg(0);
            idexwrite.setregwrite(1);
        }
        else if(opcode == 0x20)
        {  // lb
            idexwrite.setregdst(0);
            idexwrite.setalusrc(1);
            idexwrite.setaluop(0);
            idexwrite.setmemread(1);
            idexwrite.setmemwrite(0);
            idexwrite.setmemtoreg(1);
            idexwrite.setregwrite(1);
        }
        else if(opcode == 0x28)
        {  // sb
            idexwrite.setalusrc(1);
            idexwrite.setaluop(00);
            idexwrite.setmemread(0);
            idexwrite.setmemwrite(1);
            idexwrite.setregwrite(0);
        }
        else
        {
            //When the instructions are 0x00000000
            // Set all control bits to Zero '0'
            idexwrite.setregdst(0);
            idexwrite.setalusrc(0);
            idexwrite.setaluop(0);
            idexwrite.setmemread(0);
            idexwrite.setmemwrite(0);
            idexwrite.setmemtoreg(0);
            idexwrite.setregwrite(0);
            
            // Set all registers, write, read function and offset to Zero '0'
            
            idexwrite.setoffset(0);
            idexwrite.setreadreg1(0);
            idexwrite.setreadreg2(0);
            idexwrite.setwritereg2(0);
            idexwrite.setwritereg1(0);
            idexwrite.setfunction(0);
        }
    }
}
  void Pipeline::EX_stage() {
    // For NOP instruction
    if(idexread.getregdst() == 0 &&
       idexread.getalusrc() == 0 &&
       idexread.getaluop() == 0 &&
       idexread.getmemread() == 0 &&
       idexread.getmemwrite() == 0 &&
       idexread.getmemtoreg() == 0 &&
       idexread.getregwrite() == 0) {
        
        exmemwrite.setmemread(0);
        exmemwrite.setmemwrite(0);
        exmemwrite.setmemtoreg(0);
        exmemwrite.setregwrite(0);
        exmemwrite.setaluresult(0);
        exmemwrite.setswvalue(0);
        exmemwrite.setwriteregnum(0);
    }
    else {
        // Pass control signals
        exmemwrite.setmemread(idexread.getmemread());
        exmemwrite.setmemwrite(idexread.getmemwrite());
        exmemwrite.setmemtoreg(idexread.getmemtoreg());
        exmemwrite.setregwrite(idexread.getregwrite());
        
        // R-type instruction
        if(idexread.getaluop() == 10 &&
           idexread.getalusrc() == 0 &&
           idexread.getregdst() == 1) {
            
            // add instruction
            if(idexread.getfunction() == 0x20) {
                exmemwrite.setaluresult(idexread.getreadreg1() + idexread.getreadreg2());
            }
            // sub instruction
            else if(idexread.getfunction() == 0x22) {
                exmemwrite.setaluresult(idexread.getreadreg1() - idexread.getreadreg2());
            }
            
            exmemwrite.setswvalue(idexread.getreadreg2());
            exmemwrite.setwriteregnum(idexread.getwritereg2());
        }
        // lb instruction
        else if(idexread.getaluop() == 0 &&
                idexread.getalusrc() == 1 &&
                idexread.getregdst() == 0) {
            
            exmemwrite.setaluresult(idexread.getreadreg1() + idexread.getoffset());
            exmemwrite.setwriteregnum(idexread.getwritereg1());
            exmemwrite.setswvalue(idexread.getreadreg2());
        }
        // sb instruction
        else if(idexread.getaluop() == 0 && idexread.getalusrc() == 1) {
            exmemwrite.setaluresult(idexread.getreadreg1() + idexread.getoffset());
            exmemwrite.setswvalue(idexread.getreadreg2());
        }
    }
}

void Pipeline::MEM_stage() {
    // Pass control signals
    memwbwrite.setmemtoreg(exmemread.getmemtoreg());
    memwbwrite.setregwrite(exmemread.getregwrite());
    memwbwrite.setaluresult(exmemread.getaluresult());
    
    // NOP
    if(exmemread.getmemread() == 0 &&
       exmemread.getmemwrite() == 0 &&
       exmemread.getmemtoreg() == 0 &&
       exmemread.getregwrite() == 0) {
        
        memwbwrite.setwriteregnum(0);
        memwbwrite.setlwdatavalue(0);
    }
    // lb instruction
    else if(exmemread.getmemread() == 1 && exmemread.getmemwrite() == 0) {
            unsigned int address = exmemread.getaluresult();
            // Add bounds checking
            if(address < mainmemory) {  // Make sure we don't access outside array bounds
                memwbwrite.setlwdatavalue(Main_Mem[address]);
                memwbwrite.setwriteregnum(exmemread.getwriteregnum());
            } else {
                // Handle error case - set to 0 or some other safe value
                memwbwrite.setlwdatavalue(0);
                memwbwrite.setwriteregnum(0);
                std::cout << "Warning: Memory access out of bounds at address 0x"
                         << std::hex << address << std::endl;
            }
        }
    // sb instruction
        else if(exmemread.getmemread() == 0 && exmemread.getmemwrite() == 1) {
            unsigned int address = exmemread.getaluresult();
            // Add bounds checking
            if(address < mainmemory) {  // Make sure we don't access outside array bounds
                Main_Mem[address] = exmemread.getswvalue();
            } else {
                std::cout << "Warning: Memory access out of bounds at address 0x"
                         << std::hex << address << std::endl;
            }
        }
        // R-type
        else {
            memwbwrite.setwriteregnum(exmemread.getwriteregnum());
        }
    }

void Pipeline::WB_stage() {
    // lb instruction
    if(memwbread.getmemtoreg() == 1 && memwbread.getregwrite() == 1) {
        Regs[memwbread.getwriteregnum()] = memwbread.getlwdatavalue();
    }
    // R-type instruction
    else if(memwbread.getmemtoreg() == 0 && memwbread.getregwrite() == 1) {
        Regs[memwbread.getwriteregnum()] = memwbread.getaluresult();
    }
    // No write back for sb instruction
}

void Pipeline::Print_out_everything() {
    // Print Registers
    cout << "\nRegisters:" << endl;
    for(int i = 0; i < 32; i++) {
        cout << "Regs[" << dec << i << "]: 0x" << hex << uppercase << Regs[i] << endl;
    }
    
    // Print IF/ID Write
    cout << "\nIF/ID Write (written to by the IF stage)" << endl;
    cout << "Instruction: 0x" << hex << uppercase << ifidwrite.getinst() << endl;
    
    // Print IF/ID Read
    cout << "\nIF/ID Read (read by the ID stage)" << endl;
    cout << "Instruction: 0x" << hex << uppercase << ifidread.getinst() << endl;
    
    // Print ID/EX Write
    cout << "\nID/EX Write (written to by the ID stage)" << endl;
    cout << "Control Signals:" << endl;
    cout << "RegDst: " << dec << idexwrite.getregdst()
         << ", ALUSrc: " << idexwrite.getalusrc()
         << ", ALUOp: " << dec << idexwrite.getaluop()
         << ", MemRead: " << idexwrite.getmemread() << endl;
    cout << "MemWrite: " << idexwrite.getmemwrite()
         << ", MemToReg: " << idexwrite.getmemtoreg()
         << ", RegWrite: " << idexwrite.getregwrite() << endl;
    cout << "Values:" << endl;
    cout << "ReadReg1Value: 0x" << uppercase << hex << idexwrite.getreadreg1() << endl
         << "ReadReg2Value: 0x" << uppercase << hex << idexwrite.getreadreg2() << endl
         << "Offset: 0x" << uppercase << hex << idexwrite.getoffset() << endl
         << "WriteReg1: " << dec << idexwrite.getwritereg1() << endl
         << "WriteReg2: " << dec << idexwrite.getwritereg2() << ", "
         << "Function: 0x" << hex << uppercase << idexwrite.getfunction() << endl;
    
    // Print ID/EX Read
    cout << "\nID/EX Read";
    cout << "Control Signals:" << endl;
    cout << "RegDst: " << dec << idexread.getregdst()
    << ", ALUSrc: " << idexread.getalusrc()
    << ", ALUOp: " << dec << idexread.getaluop()
    << ", MemRead: " << idexread.getmemread() << endl;
    cout << "MemWrite: " << idexread.getmemwrite()
    << ", MemToReg: " << idexread.getmemtoreg()
    << ", RegWrite: " << idexread.getregwrite() << endl;
    cout << "Values:" << endl;
    cout << "ReadReg1Value: 0x" << uppercase << hex << idexread.getreadreg1() << endl
    << "ReadReg2Value: 0x" << uppercase << hex << idexread.getreadreg2() << endl
    << "Offset: 0x" << uppercase << hex << idexread.getoffset() << endl
    << "WriteReg1: " << dec << idexread.getwritereg1() << endl
    << "WriteReg2: " << dec << idexread.getwritereg2() << ", "
    << "Function: 0x" << hex << uppercase << idexread.getfunction() << endl;
    
    // Print EX/MEM Write
    cout << "\nEX/MEM Write";
    cout << "\nMemRead: " << exmemwrite.getmemread()
    << ", " << "Memwrite: " << exmemwrite.getmemwrite()
    << ", " << "MemToReg: " << exmemwrite.getmemtoreg()
    << ", " << "RegWrite: " << exmemwrite.getregwrite() << endl;
    cout << "AluResult: 0x" << uppercase << hex << exmemwrite.getaluresult()
    << ", " << "SWValue: 0x" << uppercase << hex <<exmemwrite.getswvalue()
    << ", " << "WriteRegNum: " << dec << exmemwrite.getwriteregnum() << "\n";
    
    // Print EX/MEM Read
    cout << "\nEX/MEM Read";
    cout << "\nMemRead: " << exmemread.getmemread()
    << ", " << "Memwrite: " << exmemread.getmemwrite()
    << ", " << "MemToReg: " << exmemread.getmemtoreg()
    << ", " << "Regwrite: " << exmemread.getregwrite() << endl;
    cout << "ALUResult: 0x" << uppercase << hex << exmemread.getaluresult()
    << ", " << "SWValue: 0x" << uppercase << hex << exmemread.getswvalue()
    << ", " << "WriteRegNum: " << dec << exmemread.getwriteregnum() << endl;
    
    // Print MEM/WB Write
    cout << "\nMEM/WB Write";
    cout << "\nMemToReg: " << memwbwrite.getmemtoreg()
    << ", " << "RegWrite: " << memwbwrite.getregwrite()
    << ", " << "LWDataValue: " << dec << memwbwrite.getlwdatavalue()
    << ", " << "ALUResults: 0x" << uppercase << hex << memwbwrite.getaluresult()
    << ", " << "WriteRegNum: " << dec << memwbwrite.getwriteregnum() << endl;
    
    // Print MEM/WB Read
    cout << "\nMEM/WB Write";
    cout << "\nMemToReg: " << memwbread.getmemtoreg()
    << ", " << "RegWrite: " << memwbread.getregwrite()
    << ", " << "LWDataValue: " << dec << memwbread.getlwdatavalue()
    << ", " << "ALUResults: 0x" << uppercase << hex << memwbread.getaluresult()
    << ", " << "WriteRegNum: " << dec << memwbread.getwriteregnum() << endl;
}

void Pipeline::Copy_write_to_read()
{
    // IF/ID Write to IF/ID Read
    ifidread.setinst(ifidwrite.getinst());
    
    // ID/EX Write to ID/EX Read
    idexread.setregdst(idexwrite.getregdst());
    idexread.setalusrc(idexwrite.getalusrc());
    idexread.setaluop(idexwrite.getaluop());
    idexread.setmemread(idexwrite.getmemread());
    idexread.setmemwrite(idexwrite.getmemwrite());
    idexread.setmemtoreg(idexwrite.getmemtoreg());
    idexread.setregwrite(idexwrite.getregwrite());
    // Copy values
    idexread.setreadreg1(idexwrite.getreadreg1());
    idexread.setreadreg2(idexwrite.getreadreg2());
    idexread.setoffset(idexwrite.getoffset());
    idexread.setwritereg1(idexwrite.getwritereg1());
    idexread.setwritereg2(idexwrite.getwritereg2());
    idexread.setfunction(idexwrite.getfunction());
    
    // EX/MEM Write to EX/MEM Read
    exmemread.setmemread(exmemwrite.getmemread());
    exmemread.setmemwrite(exmemwrite.getmemwrite());
    exmemread.setmemtoreg(exmemwrite.getmemtoreg());
    exmemread.setregwrite(exmemwrite.getregwrite());
    exmemread.setaluresult(exmemwrite.getaluresult());
    exmemread.setswvalue(exmemwrite.getswvalue());
    exmemread.setwriteregnum(exmemwrite.getwriteregnum());
    
    // MEM/WB Write to MEM/WB Read
    memwbread.setmemtoreg(memwbwrite.getmemtoreg());
    memwbread.setregwrite(memwbwrite.getregwrite());
    memwbread.setlwdatavalue(memwbwrite.getlwdatavalue());
    memwbread.setaluresult(memwbwrite.getaluresult());
    memwbread.setwriteregnum(memwbwrite.getwriteregnum());
}
