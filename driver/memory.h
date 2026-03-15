#define  HEAP_START 0x00100000
#define  HEAP_END 0x00900000
#define  HEAP_SIZE (HEAP_END-HEAP_START)
#define  USED 0X1A1A1A
#define  FREE 0X0F0F0F

#include <stdint.h>

struct memory_block{
    uint32_t magic_value;
    uint32_t size;
};
void init_struct(void);
void *kmalloc(int size);
void free(void * adress);
void merge(uint32_t adress);