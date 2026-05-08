#include <stdint.h>

typedef struct game_case{
    uint8_t width;
    uint8_t height;
    uint8_t* color;
} game_case_t;

typedef struct case_descriptor{
    uint8_t sprite;
    uint8_t rotation;

}case_descriptor_t;