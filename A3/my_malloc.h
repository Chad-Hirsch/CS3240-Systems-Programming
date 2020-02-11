#ifndef __MY_MALLOC_H__
#define __MY_MALLOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _block_t {
	int prepad;
	size_t           size;
	struct _block_t *prev;
	struct _block_t *next;
	char ptr[];
} block_t;

void *my_malloc(size_t size);
void  my_free(void *ptr);
void *my_calloc(size_t count, size_t size);
void *my_realloc(void *ptr, size_t size);

#endif

