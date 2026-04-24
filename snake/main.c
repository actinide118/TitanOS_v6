#include "main.h"
#include "../driver/memory.h"
#include "../util/alea.h"
#include "../applis/terminal.h"
#include "../driver/vga.h"
#include "../gui/window.h"
#include "../kernel.h"
#include "transition.h"
#include "sprite_supplier.h"

uint8_t tab[77]={
    2,2,4,4,4,2,4,4,4,4,4,
    2,2,4,4,2,2,2,4,4,4,4,
    4,4,2,2,4,2,4,2,2,4,4,
    4,2,2,4,4,2,4,4,2,2,4,
    2,2,4,4,4,2,4,4,4,2,2,
    4,4,4,4,4,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,4,4,4,
    };

game_case_t sprite={
    .width=11,
    .height=7,
    .color=tab
};

uint8_t crindex=0;

void clback(uint32_t tick){
    if(!(tick%10==0)){
        return;
    }
    fonction_transition(&sprite,3,crindex,30,10);
    /*fonction_transition(get_sprite(EMPTY_CASE),0,crindex,10,10);
    fonction_line_by_line(&sprite,1,crindex,30,10);
    fonction_line_by_line(&sprite,2,crindex,70,10);
    fonction_line_by_line(&sprite,3,crindex,90,10);*/
    crindex++;
}

void init_board(){
    game_case_t* bckg = get_sprite(EMPTY_CASE);
    game_case_t* bn = get_sprite(BONUS_CASE);
    for(uint8_t i=0;i<SNAKE_HEIGHT;i++){
        for(uint8_t j=0;j<SNAKE_WIDTH;j++){
            uint32_t rnd = random(get_seed());
            set_seed(rnd);
            if(rnd>3865470566){
                M13h_put_color_bitmap(j*16,4+i*16,16,16,bn->color);
            }else{
                M13h_put_color_bitmap(j*16,4+i*16,16,16,bckg->color);
            }
        }
    }
}

void Snake_init(){
    disable_cursor();
    clear_screen(0);
    M13h_draw_rectangle(0,0,420,4,2);
    M13h_draw_rectangle(0,196,420,4,2);
    get_seed();
    init_board();
    set_callback_tick(clback);
}