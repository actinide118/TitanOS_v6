#include <stdint.h>
#include <stddef.h>
typedef struct character{
    char letter;
    bool graph[80];
    uint8_t graph_width;
} character;

character* get_character(char input);