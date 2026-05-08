#include "transition.h"
#include "../driver/vga.h"
#include "../driver/memory.h"
#include "../util/util.h"

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


uint8_t get_bit(uint8_t* tab,uint32_t x, uint32_t y ,uint8_t width){
    return tab[y*width+x];
}
void set_bit(uint8_t* tab,uint32_t x, uint32_t y ,uint8_t width,uint8_t value){
    tab[y*width+x]=value;
}

uint32_t test_index=0;

void rotate_bitmap(game_case_t* mere,game_case_t* fille){
    fille->width=mere->height;
    fille->height=mere->width;

    for(uint32_t y=0; y<fille->height; y++){
        for(uint32_t x=0;x<fille->width;x++){
            set_bit(fille->color,x,y,fille->width,get_bit(mere->color,y,mere->width-x-1,mere->width));
        }
    }
    test_index++;
    
}

game_case_t* duplicate_bitmap(game_case_t* mere){
    game_case_t* fille=(game_case_t*)kmalloc(sizeof(game_case_t));
    fille->color=(uint8_t*)kmalloc(mere->width*mere->height*sizeof(uint8_t));
    fille->width=mere->width;
    fille->height=mere->height;
    for(uint32_t i=0;i<mere->width*mere->height-1;i++){
        fille->color[i]=mere->color[i];
    }
    return fille;
}

game_case_t* rotate_bitmap_nb(game_case_t* mere,uint8_t nb){
    if(nb==0){
        return mere;
    }
    game_case_t* imere=mere;
    bool is_first_turn = true;
    game_case_t* fille=(game_case_t*)kmalloc(sizeof(game_case_t));
    fille->color=(uint8_t*)kmalloc(mere->width*mere->height*sizeof(uint8_t));
    for(uint8_t i=0;i<nb;i++){
        rotate_bitmap(imere,fille);;
        if(!is_first_turn){
            free(imere->color);
            free(imere);
        }
        is_first_turn=false;
        imere=duplicate_bitmap(fille);
        
    }
    free(imere->color);
    free(imere);
    return fille;
}

uint8_t get_needed_rotation(uint8_t from,uint8_t to){
    if(to%2==from%2){
        return 0;
    }
    switch(from){
        case 0:
            if(to==1){
                return 1;
            }else{
                return 2;
            }
            break;
        case 1:
            if(to==0){
                return 3;
            }else{
                return 2;
            }
            break;
        case 2:
            if(to==1){
                return 0;
            }else{
                return 3;
            }
            break;
        case 3:
            if(to==0){
                return 0;
            }else{
                return 1;
            }
            break;
    }
    
    return 0;
}