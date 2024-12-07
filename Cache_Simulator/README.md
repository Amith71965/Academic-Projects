# Cache Memory Simulator

## Overview
A direct-mapped cache simulator implemented in C++ that demonstrates the fundamental concepts of cache memory and memory hierarchy in computer architecture. The simulator implements a 2KB main memory system with a 16-slot cache, featuring key cache management functionalities including read/write operations, hit/miss detection, and memory synchronization.

## Cache Architecture
- **Main Memory**: 2048 bytes (2KB)
- **Cache Size**: 16 slots
- **Cache Line Structure**:
  - Valid Bit (1 bit)
  - Tag Field
  - Dirty Bit (1 bit)
  - Data Block (16 bytes)
- **Addressing**: Direct-mapped cache with:
  - Tag bits (upper bits)
  - Slot bits (middle 4 bits)
  - Offset bits (lower 4 bits)

## Features
- Direct-mapped cache implementation
- Read and write operations with hit/miss detection
- Cache coherency management using dirty bit
- Block replacement mechanism
- Interactive command-line interface
- Real-time cache status display
- Main memory synchronization

## Technical Details
### Memory Organization
- **Main Memory Size**: 2048 bytes (0x000 to 0x7FF)
- **Cache Structure**: 16 slots × 20 bytes per slot
- **Block Size**: 16 bytes of data per cache line
- **Address Mapping**:
  ```
  Address Format: [Tag][Slot][Offset]
  - Tag: Upper bits
  - Slot: 4 bits (16 slots)
  - Offset: 4 bits (16 bytes per block)
  ```

### Cache Line Format
```
[Slot | Valid | Tag | Dirty | Data (16 bytes)]
```

## Implementation
### Key Classes
- `Cache`: Main class implementing cache functionality
  - Memory management
  - Cache operations
  - Address calculations
  - Data transfer

### Core Functions
- `readcache()`: Read data from cache/memory
- `writecache()`: Write data to cache
- `displaycache()`: Show current cache contents
- `copydata()`: Transfer data between memory and cache
- `updatemainmem()`: Synchronize cache with main memory

## Usage
### Building the Project
```bash
g++ main.cpp cachesimulator.cpp -o cache_simulator
```

### Running the Simulator
```bash
./cache_simulator
```

### Available Commands
```
(R)ead: Read from a memory address
(W)rite: Write to a memory address
(D)isplay: Show cache contents
(Q)uit: Exit simulator
```

### Example Operations
1. **Reading from Memory**:
   ```
   (R)ead, (W)rite, (D)isplay cache, (Q)uit: R
   What address would you like to read? 0x123
   ```

2. **Writing to Memory**:
   ```
   (R)ead, (W)rite, (D)isplay cache, (Q)uit: W
   What address would you like to write to? 0x123
   What data would you like to write at that address? 0xFF
   ```

3. **Displaying Cache**:
   ```
   (R)ead, (W)rite, (D)isplay cache, (Q)uit: D
   Slot Valid Tag Dirty Data
   00    0    00   0    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
   ...
   ```

## Cache Behavior
- **Cache Hit**: Data found in cache, immediate access
- **Cache Miss**: Data not in cache, requires main memory access
- **Write Policy**: Write-back with dirty bit
- **Replacement Policy**: Direct-mapped (fixed locations)

## Project Structure
```
.
├── main.cpp             # Main program entry
├── cachesimulator.cpp   # Cache implementation
├── cachesimulator.hpp   # Cache class definition
└── README.md           # Project documentation
```

## Educational Value
This simulator helps demonstrate:
- Cache memory organization
- Memory hierarchy concepts
- Address mapping techniques
- Cache hit/miss scenarios
- Memory coherency management
- Write-back policies
- Direct mapping implementation

## Future Improvements
- Implement set-associative mapping
- Add different replacement policies
- Incorporate cache performance statistics
- Add visualization of memory access patterns
- Support for different cache sizes
- GUI interface

## Contributing
Contributions are welcome! Please feel free to submit pull requests with improvements or bug fixes.
