/*
 * =====================================================================================
 *
 *       Filename:  memory.c
 *
 *    Description:  File containing method to allocate and free memory from the kernel heap  (defined in memory.h)
 *
 * 	The layout look like that:
 *	|BLOCK DESCRIPTOR|block|BLOCK DESCRIPTOR|block|...
 *
 *        Version:  1.0
 *        Created:  14/06/2026 21:28:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "memory.h"
#include "../util/util.h"
#include "vga.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_struct
 *  Description:  It initialyse the heap with a single free block the size of the heap
 * =====================================================================================
 */
void init_struct(void){
    struct memory_block* first_block=(struct memory_block* )HEAP_START;
    first_block->size=HEAP_SIZE-sizeof(struct memory_block);
    first_block->magic_value=FREE;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  crawl
 *  Description:  It start with the first block and go block descriptor to block descriptor and return the first to have a magic value set to FREE
 * =====================================================================================
 */
void *crawl(uint32_t adress){
    uint32_t i_adress=adress;
    struct memory_block* current_block=(struct memory_block* )adress;
    while(true){
        if(current_block->magic_value!=FREE&&current_block->magic_value!=USED){
            print_string("Memory corruption detected at address : ");
            char buffer[10];
            uint32_to_string(i_adress,buffer);
            print_string(buffer);
            return 0;
        }
        if(current_block->magic_value==FREE){
            return current_block;
        }
        if(current_block->size+i_adress>=HEAP_END){
            return 0;
        }
        i_adress=i_adress+current_block->size+sizeof(struct memory_block);
        current_block=(struct memory_block* )i_adress;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  split_memory
 *  Description:  It splits a memory block in two block with the first one's size being defined in the arguments of the function
 * =====================================================================================
 */
void split_memory(uint32_t adress_block,int size){
    struct memory_block* first_block=(struct memory_block* )adress_block;
    int i_size = first_block->size;
    first_block->size=size;
    struct memory_block* next_block=(struct memory_block* )(adress_block+size+sizeof(struct memory_block));
    next_block->magic_value=FREE;
    next_block->size = i_size-size-sizeof(struct memory_block*);
    return;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  merge
 *  Description:  Merge two block in one bigger
 * =====================================================================================
 */
void merge(uint32_t adress){
    struct memory_block* first_block=(struct memory_block* )adress;
    uint32_t i_size=first_block->size;
    struct memory_block* next_block=(struct memory_block* )(adress+i_size+sizeof(struct memory_block));
    first_block->size+=(next_block->size+sizeof(struct memory_block));
    next_block->size=0;
    next_block->magic_value=0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check
 *  Description:  It look all the heap blocks and merge adjacent free one 
 * =====================================================================================
 */
void check(){
    struct memory_block* first_block=(struct memory_block* )HEAP_START;
    if(first_block->size+HEAP_START>=HEAP_END){
        return;
    }
    struct memory_block* next_block=(struct memory_block* )(HEAP_START+first_block->size+sizeof(struct memory_block));
    while(true){
        if(first_block->magic_value==FREE && next_block->magic_value==FREE){
            merge((uint32_t)first_block);
            next_block=(struct memory_block* )((uint32_t)first_block+first_block->size+sizeof(struct memory_block));;
            continue;
        }
        first_block=next_block;
        if(first_block->size+(uint32_t)first_block+sizeof(struct memory_block)>=HEAP_SIZE){
            return;
        }
        next_block=(struct memory_block* )((uint32_t)first_block+first_block->size+sizeof(struct memory_block));
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  kmalloc
 *  Description:  Use the crawl function until it find a free block large enough and use the split_memory function to allocate an adress space which size was specified during the function call and return a pointer to the start of the block
 * =====================================================================================
 */
void *kmalloc(int size){
    uint32_t adress = HEAP_START;
    struct memory_block* current_block;
    while (true){
        adress = (uint32_t)crawl(adress);
        if(adress == 0){
            return 0;
        }
        current_block=(struct memory_block* )adress;
        if(current_block->size >= (size)){
            if(current_block->size >= (size+sizeof(struct memory_block)+10)){
                split_memory(adress,size);
            }
            current_block->magic_value=USED;
            return (void *)adress+sizeof(struct memory_block);
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  free
 *  Description:  It set the magic value to free in the block descriptor corresponding to the pointer passed in argument and run the check function
 * =====================================================================================
 */
void free(void * adress){
    struct memory_block*block=(struct memory_block* )((uint32_t)adress-sizeof(struct memory_block));
    block->magic_value=FREE;
    check();
}
