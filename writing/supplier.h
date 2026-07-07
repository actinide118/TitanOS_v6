#include <stdint.h>
#include <stddef.h>
typedef struct character{
    char letter;
    uint8_t graph[10];
    uint8_t graph_width;
} character;

character* get_character(char input);