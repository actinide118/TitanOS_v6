#include "main.h"
#include "../driver/memory.h"
#include "../util/alea.h"
#include "../applis/terminal.h"
#include "../driver/vga.h"
#include "../gui/window.h"
#include "../kernel.h"
#include "transition.h"
#include "sprite_supplier.h"
#include "../util/util.h"

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


case_descriptor_t* cases_status;
uint8_t x_pos,y_pos,direction,key_direction,crindex=0;
uint32_t rnd;
bool game_over=false;
case_descriptor_t* next_case;
uint16_t* snake_pieces;
uint16_t snake_size=1;

void spawn_bonus(){
    rnd = random(get_seed());
    set_seed(rnd);
    if(rnd>2147483648){
        return;
    }
    rnd = random(get_seed());
    set_seed(rnd);
    uint8_t x=rnd%SNAKE_WIDTH;
    rnd = random(get_seed());
    set_seed(rnd);
    uint8_t y=rnd%SNAKE_HEIGHT;
    uint16_t start=y*SNAKE_WIDTH+x;
    uint16_t index=start;
    bool found=false;
    bool is_not_full=false;
    while(!found){
        if(cases_status[index].sprite==BONUS_CASE){
            is_not_full=true;
        }
        if(cases_status[index].sprite==EMPTY_CASE){
            is_not_full=true;
            found=true;
            break;
        }
        index++;
        index=index%(SNAKE_HEIGHT*SNAKE_WIDTH);
        if(index==start){
            print_string("you win");
            return;
        }
    }
    if(found){
        M13h_put_color_bitmap((index%SNAKE_WIDTH)*16,4+(index/SNAKE_WIDTH)*16,16,16,get_sprite(BONUS_CASE)->color);
        cases_status[index].sprite=EMPTY_CASE;
        cases_status[index].rotation=0;
    }

}

void Snake_end(){
    free(cases_status);
    free(snake_pieces);
    
    char buf [15];
    buf[0]='s';
    buf[1]='c';
    buf[2]='o';
    buf[3]='r';
    buf[4]='e';
    buf[5]=':';
    buf[6]=' ';
    uint16_to_string(snake_size-1,buf+7);
    Term_ret(0, buf);
}

void clback(uint32_t tick){
    if(tick%10==0){
        if(game_over){
            return;
        }
        spawn_bonus();
        //key_direction=1;
        cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite=SNAKE_DROIT;
        cases_status[y_pos*SNAKE_WIDTH+x_pos].rotation=direction;
        if(direction==key_direction){
            M13h_put_color_bitmap((x_pos)*16,4+(y_pos)*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_DROIT),direction+1)->color);
        }else{
            if(direction%2==key_direction%2){
                M13h_put_color_bitmap((x_pos)*16,4+(y_pos)*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_DROIT),direction)->color);
            }else{ 
                M13h_put_color_bitmap((x_pos)*16,4+(y_pos)*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_COUDE),get_needed_rotation(direction,key_direction))->color);
            }
        }
        direction=key_direction;
        switch(direction){
            case 0:
                y_pos--;
                break;
            case 1:
                x_pos++;
                break;
            case 2:
                y_pos++;
                break;
            case 3:
                x_pos--;
                break;
        }
        if(x_pos>=SNAKE_WIDTH){
            Snake_end();
            game_over=true;
            return;
        }
        if(y_pos>=SNAKE_HEIGHT){
            Snake_end();
            game_over=true;
            return;
        }
        snake_pieces[snake_size-1]=y_pos*SNAKE_WIDTH+x_pos;
        if(cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite==BONUS_CASE){
            //print_string("bonus");*
            snake_size++;
        }else if(cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite==SNAKE_HEAD || cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite==SNAKE_COUDE || cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite==SNAKE_DROIT){
            Snake_end();
            game_over=true;
            return;
        }else{
            
            uint16_t tail_index=snake_pieces[0];
            for(uint16_t i=0;i<snake_size-1;i++){
                snake_pieces[i]=snake_pieces[i+1];
            }
            cases_status[tail_index].sprite=EMPTY_CASE;
            cases_status[tail_index].rotation=0;
            M13h_put_color_bitmap((tail_index%SNAKE_WIDTH)*16,4+(tail_index/SNAKE_WIDTH)*16,16,16,get_sprite(EMPTY_CASE)->color);
        }
        cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite=SNAKE_HEAD;
        cases_status[y_pos*SNAKE_WIDTH+x_pos].rotation=direction;
        M13h_put_color_bitmap((x_pos)*16,4+(y_pos)*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_HEAD),direction)->color);
    }

    //M13h_put_color_bitmap(x_pos*16,4+y_pos*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_HEAD),direction)->color);
    
    //fonction_transition(rotate_bitmap_nb(get_sprite(SNAKE_HEAD),crindex/16),(crindex/16)%4,crindex%16,30,10);
    /*fonction_transition(get_sprite(EMPTY_CASE),0,crindex,10,10);
    fonction_line_by_line(&sprite,1,crindex,30,10);
    fonction_line_by_line(&sprite,2,crindex,70,10);
    fonction_line_by_line(&sprite,3,crindex,90,10);*/
    crindex++;
    crindex=crindex%16;
}

void key_call(struct keymap km, uint8_t arg2){
    if(km.normal=='z' || km.normal==KEY_UP){
        key_direction=0;
    }else if(km.normal=='d' || km.normal==KEY_RIGHT){
        key_direction=1;
    }else if(km.normal=='s' || km.normal==KEY_DOWN){
        key_direction=2;
    }
    else if(km.normal=='q' || km.normal==KEY_LEFT){
        key_direction=3;
    }
}

void init_board(){
    game_case_t* bckg = get_sprite(EMPTY_CASE);
    game_case_t* bn = get_sprite(BONUS_CASE);
    for(uint8_t i=0;i<SNAKE_HEIGHT;i++){
        for(uint8_t j=0;j<SNAKE_WIDTH;j++){
            rnd = random(get_seed());
            set_seed(rnd);
            if(rnd>3865470566){
                cases_status[i*SNAKE_WIDTH+j].sprite=BONUS_CASE;
                cases_status[i*SNAKE_WIDTH+j].rotation=0;
                M13h_put_color_bitmap(j*16,4+i*16,16,16,bn->color);
            }else{
                cases_status[i*SNAKE_WIDTH+j].sprite=EMPTY_CASE;
                cases_status[i*SNAKE_WIDTH+j].rotation=0;
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
    cases_status=(case_descriptor_t*)kmalloc(SNAKE_WIDTH*SNAKE_HEIGHT*sizeof(case_descriptor_t));
    snake_pieces=(uint16_t*)kmalloc(SNAKE_WIDTH*SNAKE_HEIGHT*sizeof(uint16_t));
    init_board();
    rnd=random(get_seed());
    x_pos = rnd % (SNAKE_WIDTH-1)+1;
    rnd=random(rnd);
    y_pos = rnd % (SNAKE_HEIGHT-1)+1;
    rnd=random(rnd);
    direction=rnd%4;
    key_direction=direction;
    set_seed(rnd);
    M13h_put_color_bitmap(x_pos*16,4+y_pos*16,16,16,rotate_bitmap_nb(get_sprite(SNAKE_HEAD),direction)->color);
    cases_status[y_pos*SNAKE_WIDTH+x_pos].sprite=SNAKE_HEAD;
    cases_status[y_pos*SNAKE_WIDTH+x_pos].rotation=direction;
    snake_pieces[0]=y_pos*SNAKE_WIDTH+x_pos;
    snake_size=2;
    set_callback_tick(clback);
    set_callback_keyboard(key_call);
}

