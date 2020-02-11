# Assignment Three: Memory Allocator

You are to implement all four functions of a memory allocator:

```c
    void *my_malloc(size_t size);

    void  my_free(void *ptr);

    void *my_calloc(size_t count, size_t size);

    void *my_realloc(void *ptr, size_t size);
```

You are provided a header and implementation file. _Do not modify the header_, only modify `my_malloc.c`.

### Structure

The data structure we will be using for this is a **doubly linked list**. The struct we will use for our heap metadata is defined as:

```c
    typedef struct _block_t {
        size_t           size;
        struct _block_t *prev;
        struct _block_t *next;
        char ptr[];
    } block_t;
```

1. The free flag is the LSB of the `size` field. `0` is an allocated block and `1` is a free block.
1. The rest of the `size` field is how large the block in front of the metadata is 
(user-requested size or greater).
1. `prev` should point to the block immediately _before_ this one.
1. `next` should point to the block immediately _after_ this one.
1. `ptr` can be used to easily get the address where user-requested memory _starts_.

Your implementation file should keep track of the `HEAD` of this list, _at least_. 
You may find it useful to track the `TAIL` as well as the current program `BREAK`.

### Features

We are implementing an implicit free blocks list, meaning that both **allocated** 
and **free** blocks are stored in the list. This will be slower, but simpler. 
When the user requests memory, you will have to iterate through the blocks from 
`HEAD` to `TAIL` searching for a suitable block. If none is found, you must move 
the program `BREAK` using the `sbrk()` function. _Only ask for 4096 bytes at a time_, 
this will limit the amount of system calls we have to make.

I have been saying that you don't have to worry about allocations over 4K, _but I lied_. 
If the user requests anything over 4K, you must move the program break that amount 
(plus the header size) to get the space that is needed. You also want to align your allocations on 8 bytes.

We also want to have coalesce adjacent free blocks and split blocks that are too large for the requested size.
 To summarize:

1. Implict free list
1. Chunk a minimum of 4K from `sbrk()`
1. Align all memory requests on 8 bytes. (i.e. if user asks for 11, block must be 16 bytes in size)
1. Split blocks that are too large on calls to `my_malloc()`
1. Merge adjacent free blocks on calls to `my_free()`

