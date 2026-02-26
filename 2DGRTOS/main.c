#include "main.h"
#include "../driver/vga.h"

struct pixel cars[]={
    {0,1,'/'},
    {1,0,'_'},
    {2,0,'_'},
    {3,1,'\\'},
    {1,1,'_'},
    {2,1,'_'},
    {0,2,'o'},
    {3,2,'o'}
};
struct pixel_array car = {
    .pixels = cars,
    .size = sizeof(cars)/ sizeof(struct pixel)
};
struct pixel gun[]={
    {1,0,'\\'},
    {2,0,'_'},
    {3,0,'_'},
    {4,0,'_'},
    {5,0,'_'},
    {0,1,'/'}
};
struct pixel_array flingue = {
    .pixels = gun,
    .size = sizeof(gun)/ sizeof(struct pixel)
};

struct pixel_array all_sprite[]={
    {   .pixels = gun,
        .size = sizeof(gun)/ sizeof(struct pixel)
    },
    {   .pixels = cars,
        .size = sizeof(cars)/ sizeof(struct pixel)
    }
};

struct sprites gun_car = {
    .sprite = all_sprite,
    .size = sizeof(cars)/ sizeof(struct pixel)
};

int global_x= 0;
int global_y = 6;

void item_renderer(struct pixel_array pxs,int x, int y){
    for (int i=0; i<pxs.size; i++){
        struct pixel p = pxs.pixels[i];
        set_char_at_video_memory(p.ch,((y+p.relative_y)*80+(x+p.relative_x))*2);
    }
}
void item_renderer_color(struct pixel_array pxs,int x, int y,int color){
    for (int i=0; i<pxs.size; i++){
        struct pixel p = pxs.pixels[i];
        set_char_colr_at_video_memory(p.ch,((y+p.relative_y)*80+(x+p.relative_x))*2,color);
    }
}
void renderer(){
    item_renderer(gun_car.sprite[1],10,5);
    item_renderer(gun_car.sprite[0],0,5);
}