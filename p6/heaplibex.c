#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

typedef struct block_info_t {
    unsigned int block_size;
	unsigned int in_use;
	void *next_free;
	void *prev_free;
} block_info_t;

typedef struct free_blocks_t {
	block_info_t * first_free;
} free_blocks_t;

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_init(void *heap, unsigned int heap_size) {
	if(heap%8 != 0) {
		heap++;
		free_blocks_t * new_blocks = (free_blocks_t *) heap;
		block_info_t * first_block = (block_info_t *) (heap + sizeof(free_blocks_t));
		new_blocks->first_free = (block_info_t*) first_block;
		first_block->block_size = heap_size - sizeof(free_blocks_t);
		first_block->next_free = 0;
		first_block->prev_free = 0;
	}
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_alloc(void *heap, unsigned int block_size) {
	free_blocks_t * new_blocks = (free_blocks_t *) heap;
        //block_info_t * first_block = (block_info_t *) (heap + sizeof(free_blocks_t));
		block_info_t * current_header = (block_info_t*) (new_blocks->first_free);

        while(current_header != 0) {
                if (((current_header->block_size) - sizeof(unsigned int)) >= block_size){
                        int alloc_size = block_size + sizeof(unsigned int);
                        while(alloc_size % 8 != 0) {
                                alloc_size++;
                        }

                        int old_block_size = current_header->block_size;
                        current_header->block_size = alloc_size;
                        int split_block_size = old_block_size - alloc_size;
                        block_info_t * current_prev_free = current_header->prev_free;
                        block_info_t * current_next_free = current_header->next_free;
                        if(split_block_size > 0) {
                                char * split_block_loc = ((char*)current_header) + current_header->block_size - 1;
                                block_info_t * split_block = (block_info_t*) split_block_loc;
                                split_block->block_size = split_block_size;
                                split_block->next_free = current_header->next_free;
                                        if(current_header == new_blocks->first_free) {
                                                new_blocks->first_free = split_block;
                                                if(current_next_free != 0) {
                                                        current_next_free->prev_free = split_block;
                                                }
                                        }
                                        else {
                                                if(current_next_free != 0) {
                                                        current_next_free->prev_free = split_block;
                                                }
                                                if(current_prev_free != 0) {
                                                        current_prev_free->next_free = split_block;
                                                }


                                        }
                        }
                        else {
                                        if(current_header == new_blocks->first_free) {
                                                new_blocks->first_free = current_header->next_free;
                                                if(current_next_free != 0) {
                                                        current_next_free->prev_free = 0;
                                                }

                                        }
                                        else {
                                                if(current_next_free != 0) {
                                                        current_next_free->prev_free = current_header->prev_free;
                                                }
                                                if(current_prev_free != 0) {
                                                        current_prev_free->next_free = current_header->next_free;
                                                }
                                        }
                        }
                        break;
                }
                current_header = current_header->next_free;
        }
        if (current_header == 0) {
            return 0;
        }
        else {
	char* userlocation = ((char*) current_header) + sizeof(unsigned int);
        return (void*)userlocation;
        }
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_release(void *heap, void *block) {

}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {

    return FAILURE;
}

