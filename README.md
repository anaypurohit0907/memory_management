<img width="708" height="168" alt="image" src="https://github.com/user-attachments/assets/325e9d29-1f06-4766-adbc-30b6815f3473" />


## Custom memory allocator

# project purpose: 
    the purpose of this project was to learn about memory managent, specifically memory allocation and truly understand how it works under the hood.

# key features:
    -helper functions to see working of the allocator: ``` bytes_used() , bytes_free(), largest_free_block() ``` 
    -blocks coalescing : neighbouring free blocks are joined to form one singular block which allows the allocator to allocate larger memory sizes too.

# implementation details:
    -data structures: struct- to define and manage a block.
                        doubly linked list- to manage the blocks in the free list
    -
