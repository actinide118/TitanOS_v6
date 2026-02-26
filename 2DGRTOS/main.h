#include <stdint.h>

struct pixel{
    int relative_x;
    int relative_y;
    char ch;
};
struct pixel_array {
    struct pixel *pixels; // Pointeur vers un tableau de struct pixel
    int size;             // Taille du tableau
};

struct sprites {
    struct pixel_array *sprite;
    int size;
};

void renderer();
void item_renderer(struct pixel_array pxs,int x, int y);
void item_renderer_color(struct pixel_array pxs,int x, int y,int color);