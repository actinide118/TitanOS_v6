#include "init.h"
#include "../driver/vga.h"
#include "../applis/terminal.h"
#include "2048sprites.h"

void init_2048_board(void){
    disable_cursor();
    clear_screen(0);
    for(uint8_t i=0;i<4;i++){
        for(uint8_t j=0;j<4;j++){
            M13h_draw_rectangle(60+j*45,i*50,45,50,7);
            M13h_draw_rectangle(65+j*45,5+i*50,35,40,15);
        }
    }
    init_sprite();
    M13h_put_color_bitmap(65,5,get_sprite2048(0)->width,get_sprite2048(0)->height,get_sprite2048(0)->color);
    M13h_put_color_bitmap(110,5,get_sprite2048(1)->width,get_sprite2048(1)->height,get_sprite2048(1)->color);
    M13h_put_color_bitmap(155,5,get_sprite2048(2)->width,get_sprite2048(2)->height,get_sprite2048(2)->color);
}