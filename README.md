<img width="626" height="238" alt="image" src="https://github.com/user-attachments/assets/f393000a-a31e-4b33-ae5a-3006bb7d31ae" />
<img width="708" height="300" alt="Screenshot From 2025-10-10 16-41-38" src="https://github.com/user-attachments/assets/5b154cb0-3bd1-49e4-9843-a9bdaec37b37" />



# Custom memory allocator

## project purpose: 
The purpose of this project is to demystify low-level memory management by implementing a custom allocator in C++. This hands-on approach exposes how memory allocation, deallocation, and fragmentation work beneath high-level abstractions, providing a foundation for systems programming and performance-aware development.


## key features:
**Real-time insights**: Helper functions (`` bytes_used(), bytes_free(), largest_free_block() ``) to inspect allocator state during execution.

**Block coalescence**: Automatic merging of adjacent free blocks to form larger blocks, reducing fragmentation and supporting large allocations.

**Manual memory pool**: All memory comes from a user-supplied buffer—no malloc or new calls once the pool is initialized.

## usage:
```
mm::init_pool(buffer, BUFFER_SIZE); //initialize the memory pool

void* ptr = mm::allocate(32); //allocate 32 bytes
     
std::cout << "free block after allocation: " << mm::bytes_free()<< std::endl; //using a helper function

  
mm::deallocate(ptr); //deallocation

    
```

## limitations:

 **Single-threaded only:** The allocator is not thread-safe and does not synchronize accesses. Using it concurrently from multiple threads without locks may result in corruption or crashes.

 **Susceptible to fragmentation:** Like all free-list allocators, heavy recycling of small blocks followed by larger allocations can lead to external fragmentation, where plenty of memory is free but not in large enough contiguous chunks for some requests.

 **No built-in OOM handling:** When the pool is exhausted, allocation requests return nullptr. Client code must check for allocation failure.


## Build/Run Instructions: 
 to compile and run :
 ```
g++ -std=c++17 main.cpp manager.cpp -o allocator-demo
./allocator-demo

 ```
