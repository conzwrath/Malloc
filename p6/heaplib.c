#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

typedef struct _heap_header_t {
	unsigned int block_size;
	unsigned int in_use;
} heap_header_t;

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_init(void *heap, unsigned int heap_size) {
	int shift = 0;
	while(((uintptr_t) heap)%8 != 0) {
		heap++;
		shift++;
	}
	
	int new_heap_size = heap_size;
	while(new_heap_size%8 != 0) {
		new_heap_size--;
	}
	int * size = (int *) heap+shift;
	*size = new_heap_size;
	heap_header_t *new_heap = (heap_header_t *) (heap+8) + shift;
	new_heap->block_size = new_heap_size-8-shift;
	new_heap->in_use = 0;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {
	if(block_size == 0) return 0;
	while(((uintptr_t) heap)%8 != 0) {
		heap++;
	}
	int *size = (int *) heap;
	int i = 0;
	heap_header_t *current_header = (heap_header_t *) (heap+8);
	while(current_header->block_size-8<block_size || current_header->in_use == 1) {
		i+=block_size;
		if(i+block_size >= *size) {
			return 0;
		}
		char* next_header = ((char *)current_header) + block_size;
		current_header = (heap_header_t *)next_header;
	}
	int alloc_size = block_size + (2*sizeof(unsigned int));
	while(alloc_size % 8 != 0) {
		alloc_size++;
	}

	int old_block_size = current_header->block_size;
	current_header->block_size = alloc_size;
	current_header->in_use = 1;
	int split_block_size = old_block_size - alloc_size;
	if(split_block_size >= 8) {
		char * split_block_loc = ((char *) current_header) + current_header->block_size;
		heap_header_t * split_block = (heap_header_t *) split_block_loc;
		split_block->block_size = split_block_size;
		split_block->in_use = 0;
	}
	
	char *userlocation = ((char*) current_header) + 8;
    return (void *)userlocation;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_release(void *heap, void *block) {
	if(block==0) return;
	while(((uintptr_t) heap)%8 != 0) {
		heap++;
	}
	char * block_user_loc = (char *) block;
	heap_header_t * block_header = (heap_header_t *) (block_user_loc-8);
	block_header->in_use = 0;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
    if(block == 0) return 0;
	while(((uintptr_t) heap)%8 != 0) {
		heap++;
	}
    char *block_user_loc = (char *) block;
    heap_header_t * block_header = (heap_header_t *) (block_user_loc-8);
    void *dest;
    if(new_size > block_header->block_size) {
	hl_release(heap, block);
	dest = memmove(hl_alloc(heap,new_size), block, block_header->block_size);
    }

    else {
	hl_release(heap, block);
	dest = memmove(hl_alloc(heap,new_size), block, new_size);
    }
    return dest;
}


