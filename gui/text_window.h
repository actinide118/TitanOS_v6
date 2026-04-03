#include "window.h"
#include "../driver/memory.h"
#include <stdint.h>
#include "text_matrice.h"
typedef struct text_window{
    struct window* fenetre;
    uint16_t curr_row;
    uint16_t curr_col;
    char** texts;
    int textlen;
}text_window_t;

text_window_t* WText_create_window(uint16_t usable_height, uint16_t usable_width, char*title,uint8_t color_border,uint8_t color_title, uint8_t color_intern, uint8_t color_foreground);
void WText_printstring(text_window_t *window,char* string);