#include <stdint.h>
struct window{
    uint16_t top;
    uint16_t left;
    uint16_t width;
    uint16_t height;
    uint16_t usable_top;
    uint16_t usable_left;
    uint16_t usable_width;
    uint16_t usable_height;
    uint8_t color_border;
    uint8_t color_title;
    uint8_t color_intern;
    uint8_t color_foreground;
    char* title;
};

bool create_window(struct window* fenetre);
void del_window(struct window* fenetre);
void W_scroll(struct window* fenetre,int ln);