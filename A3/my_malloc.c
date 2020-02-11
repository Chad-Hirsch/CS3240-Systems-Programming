#include "my_malloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#define PADDING  (sizeof(block_t) + sizeof(double) - 1)

#ifdef DEBUG
#define DPRINTF(arg) fprintf(stderr, arg)
#else
#define DPRINTF(args) if (0) {}
#endif


block_t *HEAD = NULL;

static block_t *available = NULL;

static void insert_avail(block_t *block)
{
        if (block == NULL)
                return;

        if (available == NULL) {
                available = block;
                available->next = available;
                available->prev = available;
        } else {
                bool merged = false;
                block_t *curr = available;
                do {
                        char *end_curr = curr->ptr + curr->size;
                        char *begin_blk = (char *)block - block->prepad;

                        if (end_curr == begin_blk) {
                                curr->size += block->prepad + sizeof(block_t) + block->size;
                                merged = true;
                                break;
                        }
                } while (curr != available);

                if (!merged) {
                        block_t *last = available->prev;
                        last->next = block;
                        available->prev = block;
                        block->prev = last;
                        block->next = available;
                } else {
                        DPRINTF("DID MERGE!\n");
                }
        }
}

static void remove_avail(block_t *block)
{
        if (block == NULL || available == NULL)
                return;

        /* removing last item */
        if (block->next == block && block->next == block) {
                available = NULL;
        } else {
                block_t *prev = block->prev;
                block_t *next = block->next;

                /* only one item left or removing item pointed to by available */
                if (prev == next || block == available)
                        available = prev;

                prev->next  = next;
                next->prev  = prev;

                block->next = block;
                block->prev = block;
        }
}

static block_t *slice(block_t *block, size_t size)
{
        if (block->size - size < sizeof(block_t) + sizeof(double))
                return NULL;

        DPRINTF("SLICING\n");

        size_t old_size = block->size;
        block->size = size;
        remove_avail(block);

        char *start_new = block->ptr + size;
        int  offset = (uintptr_t)start_new % sizeof(double);
        size_t new_size = old_size - size - offset - sizeof(block_t);

        block_t *new_block = start_new + offset;
        new_block->prepad = offset;
        new_block->size   = new_size;
        new_block->next   = new_block;
        new_block->prev   = new_block;

        insert_avail(new_block);

        return block;
}

static block_t *find_available(size_t size)
{
        if (available == NULL)
                return NULL;

        block_t *curr = available;
        do {
                if (curr->size == size) {
                        DPRINTF("same size!\n");
                        remove_avail(curr);
                        return curr;
                } else if (curr->size > size) {
                        DPRINTF("larger size!\n");
                        return slice(curr, size);
                }
                curr = curr->next;
        } while (curr != available && curr != NULL);

        return NULL;
}


void *my_malloc(size_t size) {

	if (size < 1)
                return NULL;
        block_t *block = find_available(size);

        if (block == NULL) {
                char *start = sbrk(size + PADDING);
                int  offset = (uintptr_t)start % sizeof(double);

                block = (void *)(start + offset);
                char  *end = sbrk(0);

        if (block == (void*)-1)
                        return NULL;

                block->prepad = offset;
                block->size = size; //4096 - (2*sizeof(block_t))-aligned_size;
                block->prev = block;
                block->next = block;
        }

        return block->ptr;
}

void  my_free(void *ptr) {

	if (ptr == NULL)
                return;

        block_t *block = (void *)((char *)ptr - sizeof(block_t));
        char    *end = sbrk(0);

        if ((char *)ptr + block->size == end) {
                sbrk(-(block->size + PADDING));
                return;
        }

        insert_avail(block);
}

void *my_calloc(size_t count, size_t size) {

	size_t total = count * size;
        char *alloc = malloc(total);

	/* if malloc fails, calloc should fail */
        if (alloc == NULL)
                return NULL;

        size_t i;
        for (i = 0; i < total; i++)
                alloc[i] = 0;

        return alloc;
}

void *my_realloc(void *ptr, size_t size) {

	/* realloc can be called with NULL, if so it should work as malloc */
        if (ptr == NULL)
                return malloc(size);

        /* get the block_t containing information about the allocated memory */
	size_t  i;
        block_t *old_alloc = (void*)((char*)ptr - sizeof(block_t));
        char    *new_alloc = malloc(size);

        if (new_alloc == NULL || ptr == NULL)
                return NULL;

        for (i = 0; i < old_alloc->size && i < size; i++)
                new_alloc[i] = old_alloc->ptr[i];

        free(ptr); /* free old memory */
        return new_alloc;
}









