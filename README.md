# Cache Simulator

This project is a cache simulator. The simulator reads a text file as a command line argument and outputs the result to a text file specified by the second command line argument. The simulator simulates 6 different designs with variables within some of the designs. More information regarding the specifics of the designs can be found below in "Cache Designs." Please note that the cache line is 32 bytes throughout this entire project.

**Note:** This project is for reference only. Any code used must be adequately acknowledged.

### Files
* Makefile
* src
  * driver.cpp : a driver file to create a simulator and run it
  * cache.h : the header file for the Cache class
  * cache.cpp : the implementation for the Cache class containing all functioning code
* traces : a folder containing 3 example traces to test the simulator with
* correct_outputs : a folder containing the expected results for each provided test

### Running the program
To run the program with the included Makefile:

To compile:
```
make
```
To run with the first included test trace:
```
make run
```
To run after compiling:
```
./driver trace_file.txt output.txt
```

### Cache Designs
* Direct map cache
  * a simple cache design of one long cache in which each instruction has a specific index and is mapped solely to that index
  * runs 4x with the following sizes: 1KB, 4KB, 16KB, and 32KB
* Set-associative cache
  * a simple cache design, similar to direct map but with each index containing multiple ways allowing for multiple memory blocks that map to the same index to be kept in the cache simultaneously. 
  * runs 4x with the following n-way associativity: 2, 4, 8, and 16
* Fully-associative cache with least recently used replacement
  * Similar to direct map in that it is one long cache with only 1 way, but there are no indices. Each entry maps to a free entry until they are full. When they are full, it implements a least recently used replacement policy and evicts the oldest entry in the cache.
  * runs 1x with a cache size of 16 KB
* Fully-associative cache with hot / cold replacement
  * Same design as the previous fully-associative cache but when the cache is full it uses a temperature design to evict the least *frequently* used cache entry.
  * runs 1x with a cache size of 16 KB
* Set-associative cache with no allocation on write miss
  * Similar to the set-associative cache design as stated before, but when a miss occurs on a write instruction, no cache entry is allocated.
  * runs 4x with the following n-way associativity: 2, 4, 8, and 16
* Set-associative cache with next line prefetching
  * Similar to the set-associative cache design as stated before, but on each instruction, the next block of memory is prefetched into the cache.
  * runs 4x with the following n-way associativity: 2, 4, 8, and 16
* Set-associative cache with prefetch on miss
  * Similar to the set-associative cache design as stated before, but when a miss occurs, the next block of memory is prefetched.
  * runs 4x with the following n-way associativity: 2, 4, 8, and 16