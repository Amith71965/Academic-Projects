//
//  Pipeline.hpp
//  Pipeline
//
//  Created by Amith Kumar Yadav K
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include <stdio.h>

// Pipeline Register structures
class IF_ID {
private:
    unsigned int inst;
    
public:
    void setinst(unsigned int i) { inst = i; }
    unsigned int getinst() { return inst; }
};

class ID_EX {
private:
    // Control signals
    int regdst;
    int alusrc;
    int aluop;
    int memread;
    int memwrite;
    int memtoreg;
    int regwrite;
    
    // Data
    int readreg1;
    int readreg2;
    int offset;
    int writereg1;  // For lb type
    int writereg2;  // For R type
    int function;
    
public:
    // Constructor to initialize values
        ID_EX() {
            regdst = alusrc = aluop = memread = memwrite = memtoreg = regwrite = 0;
            readreg1 = readreg2 = offset = writereg1 = writereg2 = function = 0;
        }
    // Control Signals Setters
    void setregdst(int r) { regdst = r; }
    void setalusrc(int a) { alusrc = a; }
    void setaluop(int a) { aluop = a; }
    void setmemread(int m) { memread = m; }
    void setmemwrite(int m) { memwrite = m; }
    void setmemtoreg(int m) { memtoreg = m; }
    void setregwrite(int r) { regwrite = r; }
    void setreadreg1(int r) { readreg1 = r; }
    void setreadreg2(int r) { readreg2 = r; }
    void setoffset(int o) { offset = o; }
    //void writereg1(int w) { writereg1 = w; }
    //void writereg2(int w) { writereg2 = w; }
    void setfunction(int f) { function = f; }
    void setwritereg1(int w) { writereg1 = w; }
    void setwritereg2(int w) { writereg2 = w; } 
        
    
    // Getters
    int getregdst() { return regdst; }
    int getalusrc() { return alusrc; }
    int getaluop() { return aluop; }
    int getmemread() { return memread; }
    int getmemwrite() { return memwrite; }
    int getmemtoreg() { return memtoreg; }
    int getregwrite() { return regwrite; }
    int getreadreg1() { return readreg1; }
    int getreadreg2() { return readreg2; }
    int getoffset() { return offset; }
    int getwritereg1() { return writereg1; }
    int getwritereg2() { return writereg2; }
    int getfunction() { return function; }
};

class EX_MEM {
private:
    // Control signals
    int memread;
    int memwrite;
    int memtoreg;
    int regwrite;
    
    // Data
    int aluresult;
    int swvalue;
    int writeregnum;
    
public:
    // Setters
    void setmemread(int m) { memread = m; }
    void setmemwrite(int m) { memwrite = m; }
    void setmemtoreg(int m) { memtoreg = m; }
    void setregwrite(int r) { regwrite = r; }
    void setaluresult(int a) { aluresult = a; }
    void setswvalue(int s) { swvalue = s; }
    void setwriteregnum(int w) { writeregnum = w; }
    
    // Getters
    int getmemread() { return memread; }
    int getmemwrite() { return memwrite; }
    int getmemtoreg() { return memtoreg; }
    int getregwrite() { return regwrite; }
    int getaluresult() { return aluresult; }
    int getswvalue() { return swvalue; }
    int getwriteregnum() { return writeregnum; }
};

class MEM_WB {
private:
    // Control signals
    int memtoreg;
    int regwrite;
    
    // Data
    int lwdatavalue;
    int aluresult;
    int writeregnum;
    
public:
    // Setters
    void setmemtoreg(int m) { memtoreg = m; }
    void setregwrite(int r) { regwrite = r; }
    void setlwdatavalue(int l) { lwdatavalue = l; }
    void setaluresult(int a) { aluresult = a; }
    void setwriteregnum(int w) { writeregnum = w; }
    
    // Getters
    int getmemtoreg() { return memtoreg; }
    int getregwrite() { return regwrite; }
    int getlwdatavalue() { return lwdatavalue; }
    int getaluresult() { return aluresult; }
    int getwriteregnum() { return writeregnum; }
};

class Pipeline {
private:
    static const int mainmemory = 1024;  // 1K memory
    static const int regsNum = 32;       // 32 registers
    
    int Main_Mem[mainmemory];           // Main memory array and used exact names as    specified in the Assignment requirement file
    int Regs[regsNum];                  // Register file and used exact names as specified in the Assignment requirement file.
    
    // Pipeline registers
    IF_ID ifidwrite, ifidread;
    ID_EX idexwrite, idexread;
    EX_MEM exmemwrite, exmemread;
    MEM_WB memwbwrite, memwbread;
    
public:
    Pipeline();  // Constructor
    
    // Pipeline stage functions
    void IF_stage(unsigned int instruction);
    void ID_stage();
    void EX_stage();
    void MEM_stage();
    void WB_stage();
    
    // Required functions
    void Print_out_everything();
    void Copy_write_to_read();
};

#endif /* Pipeline_hpp */
