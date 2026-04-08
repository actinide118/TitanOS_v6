#include "window.h"
#include "../driver/memory.h"
#include <stdint.h>
typedef struct text_window{
    struct window* fenetre;
    uint16_t curr_row;
    uint16_t curr_col;
}text_window_t;

text_window_t* WText_create_window(uint16_t usable_height, uint16_t usable_width, char*title,uint8_t color_border,uint8_t color_title, uint8_t color_intern, uint8_t color_foreground);
uint8_t WText_printstring(text_window_t *window,char* string);
void WText_erase_last_line(text_window_t *window);
void WText_erase_last_char(text_window_t* window);
uint8_t WText_printstring_color(text_window_t *window,char* string,uint8_t foreground,uint8_t background);