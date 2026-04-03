#include "text_window.h"
#include "../writing/supplier.h"
#include "../util/util.h"
#include "../driver/vga.h"

text_window_t* WText_create_window(uint16_t usable_height, uint16_t usable_width, char*title,uint8_t color_border,uint8_t color_title, uint8_t color_intern, uint8_t color_foreground){
    text_window_t* t_window=(text_window_t*)kmalloc(sizeof(text_window_t));
    struct window* i_window=(struct window*)kmalloc(sizeof(struct window));
    i_window->usable_width=usable_width;
    i_window->usable_height=usable_height;
    i_window->title=title;
    i_window->color_border=color_border;
    i_window->color_foreground=color_foreground;
    i_window->color_intern=color_intern;
    i_window->color_title=color_title;
    create_window(i_window);
    t_window->fenetre=i_window;
    t_window->curr_col=0;
    t_window->curr_row=0;
    uint8_t col=usable_width/3;
    uint8_t row=usable_height/10;
    t_window->texts=(char**)kmalloc(col*row*sizeof(char));
    t_window->textlen=col*row;
    return t_window;
}

void Intern_WText_putchar(text_window_t *window,char ch){
    if(ch=='\n'){
        if(window->curr_row+10>=window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
        return;
    }
    struct character *graphic_char=get_character(ch);
    if(graphic_char->graph_width+window->curr_col>window->fenetre->usable_width){
        if(window->curr_row+10>=window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
    }
    M13h_put_binary_bitmap(window->fenetre->usable_left+window->curr_col,window->fenetre->usable_top+window->curr_row,graphic_char->graph_width,10,window->fenetre->color_foreground,window->fenetre->color_intern,graphic_char->graph);
    window->curr_col+=graphic_char->graph_width;
}

void WText_printstring(text_window_t *window,char* string){
    while(*string){
        Intern_WText_putchar(window,*string);
        *string++;
    }
    return;
}