# MIPS Pipeline Simulator

A C++ implementation of a 5-stage MIPS pipeline simulator that demonstrates the working of a pipelined datapath. This simulator implements basic MIPS instructions and shows the state of pipeline registers at each clock cycle.

## Overview

This simulator implements a 5-stage pipeline with the following stages:
- Instruction Fetch (IF)
- Instruction Decode (ID)
- Execute (EX)
- Memory Access (MEM)
- Write Back (WB)

### Supported Instructions
- `add` (R-type)
- `sub` (R-type)
- `lb` (Load Byte)
- `sb` (Store Byte)
- `nop` (No Operation)

### Features
- Simulates a 1K Main Memory
- 32 General Purpose Registers
- Complete pipeline register state display
- Cycle-by-cycle execution

## Implementation Details

### Memory and Register Organization
- Main Memory: 1024 bytes (1K), initialized with values 0x00 to 0xFF repeating
- Registers: 32 registers (R0-R31)
  - R0 is hardwired to 0
  - Other registers initialized to 0x100 + register_number

### Pipeline Registers
- IF/ID (Instruction Fetch to Instruction Decode)
- ID/EX (Instruction Decode to Execute)
- EX/MEM (Execute to Memory Access)
- MEM/WB (Memory Access to Write Back)

### Instruction Formats
```
R-type: add, sub
Opcode (6 bits) | rs (5 bits) | rt (5 bits) | rd (5 bits) | shamt (5 bits) | funct (6 bits)

I-type: lb, sb
Opcode (6 bits) | rs (5 bits) | rt (5 bits) | immediate (16 bits)
```

## Project Structure
```
├── Pipeline.hpp         # Class declarations and pipeline register structures
├── Pipeline.cpp         # Implementation of pipeline stages and main logic
└── main.cpp            # Program entry point and test instructions
```

## Building and Running

### Prerequisites
- C++ compiler supporting C++11 or later
- Make (optional, for build automation)

### Compilation
```bash
g++ -std=c++11 main.cpp Pipeline.cpp -o pipeline_simulator
```

### Running
```bash
./pipeline_simulator
```

## Output Format

The simulator outputs the following information for each clock cycle:

1. Register values (all 32 registers)
2. Pipeline register states:
   - IF/ID Write and Read
   - ID/EX Write and Read
   - EX/MEM Write and Read
   - MEM/WB Write and Read

Example output format:
```
Clock Cycle 1
----------------
Registers:
Regs[0]: 0x0
Regs[1]: 0x101
...
Regs[31]: 0x11F

IF/ID_Write (written to by the IF stage)
Inst = 0xA1020000 [sb]
...
```

## Test Program

The simulator includes a test program with the following instructions:
```
0xa1020000    // sb $2, 0($8)
0x810AFFFC    // lb $10, -4($8)
0x00831820    // add $3, $4, $3
0x01263820    // add $7, $9, $6
0x01224820    // add $9, $9, $2
0x81180000    // lb $24, 0($8)
0x81510010    // lb $17, 16($10)
0x00624022    // sub $8, $3, $2
0x00000000    // nop
0x00000000    // nop
0x00000000    // nop
0x00000000    // nop
```

## Notes
- The simulator does not handle data hazards
- Branch instructions are not implemented
- Memory access is byte-addressable
- Pipeline register contents are displayed before copying from write to read

## Author
Amit Kumar Yadav K
