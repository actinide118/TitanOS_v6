#include "text_window.h"
#include "../writing/supplier.h"
#include "../util/util.h"
#include "../driver/vga.h"

uint8_t last_char_width=0;

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
    return t_window;
}

bool Intern_WText_putchar(text_window_t *window,char ch){
    if(ch=='\n'){
        if(window->curr_row+20>window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
        return true;
    }
    bool haschangedline=false;
    struct character *graphic_char=get_character(ch);
    if(graphic_char->graph_width+window->curr_col>window->fenetre->usable_width){
        if(window->curr_row+10>=window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
        haschangedline=true;
    }
    M13h_put_binary_bitmap(window->fenetre->usable_left+window->curr_col,window->fenetre->usable_top+window->curr_row,graphic_char->graph_width,10,window->fenetre->color_foreground,window->fenetre->color_intern,graphic_char->graph);
    window->curr_col+=graphic_char->graph_width;
    last_char_width=graphic_char->graph_width;
    return haschangedline;
}

uint8_t WText_printstring(text_window_t *window,char* string){
    uint8_t chgmt=0;
    while(*string){
        if(Intern_WText_putchar(window,*string)){
            chgmt++;
        }
        *string++;
    }
    return chgmt;
}

void WText_erase_last_line(text_window_t *window){
    if(window->curr_col==0&&window->curr_row>0){
        window->curr_row-=10;
    }
    M13h_draw_rectangle(window->fenetre->usable_left,window->fenetre->usable_top+window->curr_row,window->fenetre->usable_width,10,(int)window->fenetre->color_intern);
    window->curr_col=0;
    last_char_width=0;
}

void WText_erase_last_char(text_window_t* window){
    M13h_draw_rectangle(window->fenetre->usable_left+window->curr_col-last_char_width,window->fenetre->usable_top+window->curr_row,last_char_width,10,window->fenetre->color_intern);
    window->curr_col-=last_char_width;
}

bool Intern_WText_putchar_color(text_window_t *window,char ch,uint8_t foreground,uint8_t background){
    if(ch=='\n'){
        if(window->curr_row+10>window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
        return true;
    }
    bool haschangedline=false;
    struct character *graphic_char=get_character(ch);
    if(graphic_char->graph_width+window->curr_col>window->fenetre->usable_width){
        if(window->curr_row+10>=window->fenetre->usable_height){
            W_scroll(window->fenetre,10);
            window->curr_col=0;
        }else{
            window->curr_row+=10;
            window->curr_col=0;
        }
        haschangedline=true;
    }
    M13h_put_binary_bitmap(window->fenetre->usable_left+window->curr_col,window->fenetre->usable_top+window->curr_row,graphic_char->graph_width,10,foreground,background,graphic_char->graph);
    window->curr_col+=graphic_char->graph_width;
    last_char_width=graphic_char->graph_width;
    return haschangedline;
}

uint8_t WText_printstring_color(text_window_t *window,char* string,uint8_t foreground,uint8_t background){
    uint8_t chgmt=0;
    while(*string){
        if(Intern_WText_putchar_color(window,*string,foreground,background)){
            chgmt++;
        }
        *string++;
    }
    return chgmt;
}

void WText_clear(text_window_t* window){
    M13h_draw_rectangle(window->fenetre->usable_left,window->fenetre->usable_top,window->fenetre->usable_width,window->fenetre->usable_height,window->fenetre->color_intern);
    M13h_draw_rectangle(window->fenetre->left,window->fenetre->top,window->fenetre->width,10,window->fenetre->color_border);
    M13h_draw_rectangle(window->fenetre->left,window->fenetre->top,4,window->fenetre->height,window->fenetre->color_border);
    M13h_draw_rectangle(window->fenetre->left,window->fenetre->top+window->fenetre->height-4,window->fenetre->width,4,window->fenetre->color_border);
    M13h_draw_rectangle(window->fenetre->left+window->fenetre->width-4,window->fenetre->top,4,window->fenetre->height,window->fenetre->color_border);
    uint16_t row = get_row();
    uint16_t col = get_col();
    set_row(window->fenetre->top);
    set_col(window->fenetre->left+4);
    uint8_t background = get_background();
    uint8_t foreground = get_foreground();
    set_background(window->fenetre->color_border);
    set_foreground(window->fenetre->color_title);
    M13h_print_string(window->fenetre->title);
    M13h_draw_rectangle(window->fenetre->usable_left,window->fenetre->usable_top,window->fenetre->usable_width,window->fenetre->usable_height,window->fenetre->color_intern);
    window->curr_col=0;
    window->curr_row=0;
}