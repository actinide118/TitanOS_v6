#include <stdint.h>
#include "../event/evt_queu.h"
#include "../driver/vga.h"
#include "../driver/keyboard2.h"
#include "../util/ascii.h"
#include "../2DGRTOS/main.h"
#include "../util/alea.h"
#include "move.h"

int x = 0;
int y = 3;

struct pixel voiture[]={
    {0,1,'/'},
    {1,0,'_'},
    {2,0,'_'},
    {3,1,'\\'},
    {1,1,'_'},
    {2,1,'_'},
    {0,2,'o'},
    {3,2,'o'}
};
struct pixel_array bagnole = {
    .pixels = voiture,
    .size = sizeof(voiture)/ sizeof(struct pixel)
};

struct pixel route_pixels[]={
        {0,0,'|'},
        {0,1,'|'},
        {0,2,'|'},
        {0,3,'|'},
        {0,4,'|'},
        {0,5,'|'},
        {0,6,'|'},
        {0,7,'|'},
        {0,8,'|'},
        {0,9,'|'},
        {0,10,'|'},
        {0,11,'|'},
        {0,12,'|'},
        {0,13,'|'},
        {0,14,'|'},
        {0,15,'|'},
        {0,16,'|'},
        {0,17,'|'},
        {0,18,'|'},
        {0,19,'|'},
        {0,20,'|'},
        {0,21,'|'},
        {0,22,'|'},
        {0,23,'|'},
        {0,24,'|'},
};

struct pixel_array route = {
    .pixels = route_pixels,
    .size = sizeof(route_pixels)/ sizeof(struct pixel)
};
struct ennemi ennemis[]={
    {0,0},
};

void mv(struct keymap ch, uint8_t flag){
    clear_screen();
    switch(ch.normal){
        case 'q':
            if (x>0){
                x--;
            }
            break;
        case 'd':
            if (x<2){
                x++;
            }
            break;            
    }
    item_renderer(bagnole,x*5+30,20);
    item_renderer(route,34,0);
    item_renderer(route,29,0);
    item_renderer(route,39,0);
    item_renderer(route,44,0);

    int sizeofennemi = sizeof(ennemis)/sizeof(struct ennemi);
    for (int i=0; i<sizeofennemi; i++){
        item_renderer_color(bagnole,ennemis[i].x*5+30, ennemis[i].y, 0x04);
    }
    
    if(ch.normal == ASCII_BS){
        reset();
    }
}

void init_game(){
    set_callback_keyboard(mv);
    clear_screen();
}