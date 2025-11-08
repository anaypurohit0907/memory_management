<img width="626" height="238" alt="image" src="https://github.com/user-attachments/assets/f393000a-a31e-4b33-ae5a-3006bb7d31ae" />



# Custom memory allocator

## project purpose: 
 the purpose of this project was to learn about memory managent, specifically memory allocation and truly understand how it works under the hood.

## key features:
-helper functions to see working of the allocator: ``` bytes_used() , bytes_free(), largest_free_block() ``` 
 -blocks coalescing : neighbouring free blocks are joined to form one singular block which allows the allocator to allocate larger memory sizes too.

## usage:
```
mm::init_pool(buffer, BUFFER_SIZE); //initialize the memory pool

void* ptr = mm::allocate(32); //allocate 32 bytes
     
std::cout << "free block after allocation1: " << mm::bytes_free()<< std::endl; //using a helper function

  
mm::deallocate(ptr); //deallocation
 ptrs[i] = nullptr;
    
```

## limitations:
-this allocator is single thereaded so it can't do multi threaded deallocation or coalescing and hence performs very bad when there is need to deallocate and coalesce lots of memory .
-fragmentation- since this is a free list based allocator, This behavior is a well-known challenge for any free-list-based allocator . sao dont try to deallocate  32 byte blocks but re-allocate more than sized blocks after.

## Build/Run Instructions: 
 to compile:
 ```
 g++ main.cpp manager.cpp
 ```
to run: 
```
./a.out
```
