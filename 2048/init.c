#include "init.h"
#include "../driver/vga.h"

void init_2048_board(void){
    M13h_draw_rectangle(0,0,320,200,0);
}