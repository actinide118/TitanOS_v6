#include "transition.h"
#include "../driver/vga.h"

void fonction_line_by_line(game_case_t* new_case,uint8_t direction,uint8_t step,uint8_t x,uint8_t y){
    if(direction > 3){
        return;
    }
    switch(direction){
        case 0:
            if(step>=new_case->height){
                return;
            }
            for(uint8_t i=0; i<new_case->width;i++){
                M13h_set_pixel(x+i,y+step,(int)new_case->color[i+step*new_case->width]);
            }
            break;
        case 1:
            if(step>=new_case->width){
                return;
            }
            for(uint8_t i=0; i<new_case->height;i++){
                M13h_set_pixel(x+(new_case->width-step),y+i,(int)new_case->color[new_case->width*(i)+(new_case->width-step-1)]);
            }
            break;
        case 2:
            if(step>=new_case->height){
                return;
            }
            for(uint8_t i=0; i<new_case->width;i++){
                M13h_set_pixel(x+i,y+(new_case->height-step-1),(int)new_case->color[i+(new_case->width*(new_case->height-step-1))]);
            }
            break;
        case 3:
            if(step>=new_case->width){
                return;
            }
            for(uint8_t i=0; i<new_case->height;i++){
                M13h_set_pixel(x+step,y+i,(int)new_case->color[new_case->width*(i)+step]);
            }
            break;
    }
}

void draw_line(game_case_t* new_case,uint8_t x, uint8_t y, bool is_horizontal,uint32_t index_start){
    if(is_horizontal){
        for(uint8_t i=0;i<new_case->width;i++){
            M13h_set_pixel(x+i,y,new_case->color[index_start+i]);
        }
    }else{
        uint32_t index=index_start;
        for(uint8_t i=0;i<new_case->height;i++){
            M13h_set_pixel(x,y+i,new_case->color[index]);
            index+=new_case->width;
        }
    }
}

void fonction_transition(game_case_t* new_case,uint8_t direction,uint8_t step,uint8_t x,uint8_t y){
    if(direction > 3){
        return;
    }
        uint32_t index;
        switch(direction){
        case 0:
            if(step>=new_case->height){
                return;
            }
            index=new_case->width*(new_case->height-step-1)-1;
            for(uint8_t i=0;i<=step;i++){
                draw_line(new_case,x,y+i,true,index);
                index=index+new_case->width;
            }
            break;
        case 1:
            if(step>=new_case->width){
                return;
            }
            index=0;
            for(uint8_t i=0; i<=step;i++){
                draw_line(new_case,x+new_case->width-step+i,y,false,index);
                index++;
            }
            break;
        case 2:
            if(step>=new_case->height){
                return;
            }
            index=0;
            for(uint8_t i=0; i<=step;i++){
                draw_line(new_case,x,y+new_case->height-step+i,true,index);
                index+=new_case->width;
            }
            break;
        case 3:
            if(step>=new_case->width){
                return;
            }
            index=new_case->width-step-1;
            for(uint8_t i=0; i<=step;i++){
                draw_line(new_case,x+i,y,false,index);
                index++;
            }
            break;
    }
}