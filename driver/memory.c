#include "memory.h"
#include "../util/util.h"
#include "vga.h"


void init_struct(void){
    struct memory_block* first_block=(struct memory_block* )HEAP_START;
    first_block->size=HEAP_SIZE-sizeof(struct memory_block);
    first_block->magic_value=FREE;
}
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

void split_memory(uint32_t adress_block,int size){
    struct memory_block* first_block=(struct memory_block* )adress_block;
    int i_size = first_block->size;
    first_block->size=size;
    struct memory_block* next_block=(struct memory_block* )(adress_block+size+sizeof(struct memory_block));
    next_block->magic_value=FREE;
    next_block->size = i_size-size-sizeof(struct memory_block*);
    return;
}

void merge(uint32_t adress){
    struct memory_block* first_block=(struct memory_block* )adress;
    uint32_t i_size=first_block->size;
    struct memory_block* next_block=(struct memory_block* )(adress+i_size+sizeof(struct memory_block));
    first_block->size+=(next_block->size+sizeof(struct memory_block));
    next_block->size=0;
    next_block->magic_value=0;
}

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

void free(void * adress){
    struct memory_block*block=(struct memory_block* )((uint32_t)adress-sizeof(struct memory_block));
    block->magic_value=FREE;
    check();
}