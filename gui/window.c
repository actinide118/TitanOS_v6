/*
 * =====================================================================================
 *
 *       Filename:  window.c
 *
 *    Description:  File containing methods to create and manage windows
 *
 *        Version:  1.0
 *        Created:  14/06/2026 20:57:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "window.h"
#include "../driver/vga.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  create_window
 *  Description:  Create a window using a struct window object and update this one. The nececessary fields are usable_width, usable_height, color_border and color_intern
 * =====================================================================================
 */
bool create_window(struct window* fenetre){// The six first fields of the struct are overwritten by the function who create the window based on the other field
    if(fenetre->usable_width+8> VGA_WIDTH||fenetre->usable_height+14>VGA_HEIGHT){
        return false;
    }
    uint16_t center_top=(fenetre->usable_height+14)>>1;
    uint16_t center_left=(fenetre->usable_width+8)>>1;
    fenetre->width=fenetre->usable_width+8;
    fenetre->height=fenetre->usable_height+16;
    fenetre->top=(VGA_HEIGHT>>1)-center_top;
    fenetre->left=(VGA_WIDTH>>1)-center_left;
    fenetre->usable_top=fenetre->top+10;
    fenetre->usable_left=fenetre->left+4;
    M13h_draw_rectangle(fenetre->left,fenetre->top,fenetre->width,10,fenetre->color_border);
    M13h_draw_rectangle(fenetre->left,fenetre->top,4,fenetre->height,fenetre->color_border);
    M13h_draw_rectangle(fenetre->left,fenetre->top+fenetre->height-4,fenetre->width,4,fenetre->color_border);
    M13h_draw_rectangle(fenetre->left+fenetre->width-4,fenetre->top,4,fenetre->height,fenetre->color_border);
    uint16_t row = get_row();
    uint16_t col = get_col();
    set_row(fenetre->top);
    set_col(fenetre->left+4);
    uint8_t background = get_background();
    uint8_t foreground = get_foreground();
    set_background(fenetre->color_border);
    set_foreground(fenetre->color_title);
    M13h_print_string(fenetre->title);
    M13h_draw_rectangle(fenetre->usable_left,fenetre->usable_top,fenetre->usable_width,fenetre->usable_height,fenetre->color_intern);
    set_row(row);
    set_col(col);
    set_background(background);
    set_foreground(foreground);
    return true;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  del_window
 *  Description:  put a rectangle the color of the background where the window was but don't update the struct window
 * =====================================================================================
 */
void del_window(struct window* fenetre){
    M13h_draw_rectangle(fenetre->top,fenetre->left,fenetre->width,fenetre->height,get_background());
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  W_scroll
 *  Description:  remove ln lines from the top, move the others to the top and add ln lines of the color_intern properties
 * =====================================================================================
 */
void W_scroll(struct window* fenetre,int ln){
    for(int i=0;i<=fenetre->usable_height-ln;i++){
        for(int j=0;j<fenetre->usable_width;j++){
            uint8_t color=M13h_get_pixel(fenetre->usable_left+j,fenetre->usable_top+i+ln);
            M13h_set_pixel(fenetre->usable_left+j,fenetre->usable_top+i,(int)color);
        }
    }
    M13h_draw_rectangle(fenetre->usable_left,fenetre->usable_top+fenetre->usable_height-ln,fenetre->usable_width,ln,(int)fenetre->color_intern);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  W_clear
 *  Description:  Clear the interior of a window but not the border not like del_window
 * =====================================================================================
 */
void W_clear(struct window* fenetre){
   M13h_draw_rectangle(fenetre->usable_left,fenetre->usable_top,fenetre->usable_width,fenetre->usable_height+2,fenetre->color_intern); 
}
