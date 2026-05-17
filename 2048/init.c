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
    /*M13h_put_color_bitmap(65,5,get_sprite2048(0)->width,get_sprite2048(0)->height,get_sprite2048(0)->color);
    M13h_put_color_bitmap(110,5,get_sprite2048(1)->width,get_sprite2048(1)->height,get_sprite2048(1)->color);
    M13h_put_color_bitmap(155,5,get_sprite2048(2)->width,get_sprite2048(2)->height,get_sprite2048(2)->color);
    M13h_put_color_bitmap(200,5,get_sprite2048(3)->width,get_sprite2048(3)->height,get_sprite2048(3)->color);
    M13h_put_color_bitmap(65,55,get_sprite2048(4)->width,get_sprite2048(4)->height,get_sprite2048(4)->color);
    M13h_put_color_bitmap(110,55,get_sprite2048(5)->width,get_sprite2048(5)->height,get_sprite2048(5)->color);
    M13h_put_color_bitmap(155,55,get_sprite2048(6)->width,get_sprite2048(6)->height,get_sprite2048(6)->color);
    M13h_put_color_bitmap(200,55,get_sprite2048(7)->width,get_sprite2048(7)->height,get_sprite2048(7)->color);
    M13h_put_color_bitmap(65,105,get_sprite2048(8)->width,get_sprite2048(8)->height,get_sprite2048(8)->color);
    M13h_put_color_bitmap(110,105,get_sprite2048(9)->width,get_sprite2048(9)->height,get_sprite2048(9)->color);
    M13h_put_color_bitmap(155,105,get_sprite2048(10)->width,get_sprite2048(10)->height,get_sprite2048(10)->color);
*/

}

