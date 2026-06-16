/*
 * =====================================================================================
 *
 *       Filename:  vga.c
 *
 *    Description:  File containing all the methods to interact with the graphics
 *
 *        Version:  1.0
 *        Created:  15/06/2026 07:46:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include "vga.h"
#include "ports.h"
#include "../util/util.h"
#include "../writing/supplier.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_set_cursor
 *  Description:  Set the position of the cursor in VGA 3h mode
 * =====================================================================================
 */
void M3h_set_cursor(int offset) {
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_get_cursor
 *  Description:  Get the position of the cursor in VGA 3h mode
 * =====================================================================================
 */
int M3h_get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_set_char_at_video_memory
 *  Description:  Put a black on white character at the right offset in VGA 3h mode
 * =====================================================================================
 */
void M3h_set_char_at_video_memory(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_get_offset
 *  Description:  Function to calculate a position in memory giving row and column in VGA 3h mode
 * =====================================================================================
 */
int M3h_get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_get_row_from_offset
 *  Description:  Get the row of a given memory position
 * =====================================================================================
 */
int M3h_get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_move_offset_to_new_line
 *  Description:  Return the memory position at the start of a new line after the given offset in VGA 3h mode
 * =====================================================================================
 */
int M3h_move_offset_to_new_line(int offset) {
    return M3h_get_offset(2, M3h_get_row_from_offset(offset) + 1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_scroll_ln
 *  Description:  Scroll of one line in VGA 3h mode
 * =====================================================================================
 */
int M3h_scroll_ln(int offset) {
    memory_copy(
            (uint8_t * )(M3h_get_offset(0, 1) + VIDEO_ADDRESS),
            (uint8_t * )(M3h_get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        M3h_set_char_at_video_memory(' ', M3h_get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_print_string
 *  Description:  Print a string to screen in VGA 3h mode
 * =====================================================================================
 */
void M3h_print_string(char *string) {
    int offset = M3h_get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = M3h_scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = M3h_move_offset_to_new_line(offset);
        } else {
            M3h_set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;
    }
    M3h_set_cursor(offset);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_set_char_colr_at_video_memory
 *  Description:  Put a char with a given color on screen
 * =====================================================================================
 */
void M3h_set_char_colr_at_video_memory(char ch,int offset, int color){
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = ch;
    vidmem[offset + 1] = color;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_print_string_color
 *  Description:  Same as M3h_print_string but with color
 * =====================================================================================
 */
void M3h_print_string_color(char *string, int color){
    int offset = M3h_get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = M3h_scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = M3h_move_offset_to_new_line(offset);
        } else {
            M3h_set_char_colr_at_video_memory(string[i], offset, color);
            offset += 2;
        }
        i++;
    }
    M3h_set_cursor(offset);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_print_nl
 *  Description:  Move offset to a new line and scroll if necessary
 * =====================================================================================
 */
void M3h_print_nl() {
    int newOffset = M3h_move_offset_to_new_line(M3h_get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = M3h_scroll_ln(newOffset);
    }
    M3h_set_cursor(newOffset);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_clear_screen
 *  Description:  Clear the VGA 3h mode screen
 * =====================================================================================
 */
void M3h_clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        M3h_set_char_at_video_memory(' ', i * 2);
    }
    M3h_set_cursor(M3h_get_offset(0, 0));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_print_backspace
 *  Description:  Erase last char put to screen
 * =====================================================================================
 */
void M3h_print_backspace() {
    int newCursor = M3h_get_cursor() - 2;
    M3h_set_char_at_video_memory(' ', newCursor);
    M3h_set_cursor(newCursor);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M3h_cadre_de_couleur
 *  Description:  Create a border around the screen of a given color in VGA 3h mode
 * =====================================================================================
 */
void M3h_cadre_de_couleur(int color){
    int i= 0;
    int offset = 0x00;
    while(i < 80){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=2;
        i++;
    }
    offset = (24*80+0)*2 ;
    i = 0;
    while(i < 80){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=2;
        i++;
    }
    offset = 0x00;
    i=0;
    while(i < 25){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=160;
        i++;
    }
    offset = 0x02;
    i=0;
    while(i < 25){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=160;
        i++;
    }
    offset = 156;
    i=0;
    while(i < 25){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=160;
        i++;
    }
    offset = 158;
    i=0;
    while(i < 25){
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = 0x00;
        vidmem[offset + 1] = color;
        offset +=160;
        i++;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  error_occured
 *  Description:  Set the border in red and show an error message
 * =====================================================================================
 */
void error_occured(char *string){
    M3h_clear_screen();
    M3h_cadre_de_couleur(BLACK_ON_RED);
    int offset = (10*80+30)*2;
    int i = 0;
    while (string[i] != 0) {
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = string[i];
        vidmem[offset + 1] = RED_ON_BLACK;
        offset += 2;
        i++;
    }
    M3h_set_cursor((10*80+30+i)*2);
}

uint8_t foreground=7;
uint8_t background=0;
uint16_t current_row = 0 ;
uint16_t current_col=0;
bool is_13h=false;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Is_13h
 *  Description:  Return if the display is currently in VGA 13h mode
 * =====================================================================================
 */
bool Is_13h(void){
    return is_13h;
}
// Misclaneous function to manage values their name mean their purpose
uint8_t get_foreground(void){
    return foreground;
}

uint8_t get_background(void){
    return background;
}

void set_foreground(uint8_t color){
    foreground=color;
    return;
}

void set_background(uint8_t color){
    background= color;
    return;
}

uint16_t get_row(void){
    return current_row;
}

uint16_t get_col(void){
    return current_col;
}

void set_col(uint16_t col){
    current_col=col;
    return;
}

void set_row(uint16_t row){
    current_row=row;
    return;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  switch_to_13h
 *  Description:  Use a list of write to the graphic card to put it in VGA 13h mode
 *  See usefulllinks.txt number 3 to see where my impletation come from
 * =====================================================================================
 */
void switch_to_13h(void){
    asm("cli");
        port_byte_out(0x3C2,0b01100011);
        port_word_out(0x03D4, 0x0E11);
        port_word_out(0x03D4, 0x5F00);
        port_word_out(0x03D4, 0x4F01);
        port_word_out(0x03D4, 0x5002);
        port_word_out(0x03D4, 0x5404);
        port_word_out(0x03D4, 0x2813);
        port_word_out(0x03D4, 0xBF06);
        port_word_out(0x03D4, 0x1F07);
        port_word_out(0x03D4, 0x4109);
        port_word_out(0x03D4, 0x9C10);
        port_word_out(0x03D4, 0x8E11);
        port_word_out(0x03D4, 0x8F12);
        port_word_out(0x03D4, 0x9615);
        port_word_out(0x03D4, 0xB916);
        port_word_out(0x03D4, 0x0008);
        port_word_out(0x03D4, 0x4014);
        port_word_out(0x03D4, 0xA317);
        port_word_out(0x03C4, 0x0E04);
        port_word_out(0x03C4, 0x0301);
        port_word_out(0x03C4, 0x0f02);
        port_word_out(0x03CE, 0x4005);
        port_word_out(0x03CE, 0x0506);
        port_byte_in(0x03DA);
        port_byte_out(0x03C0, 0x30);
        port_byte_out(0x03C0, 0x41);
        port_byte_out(0x03C0, 0x33);
        port_byte_out(0x03C0, 0x00);
        is_13h=true;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_set_pixel
 *  Description:  Set a pixel with a given color at the given coordonates
 * =====================================================================================
 */
void M13h_set_pixel(int x, int y, int color){
    if(x < 0 || x>= VGA_WIDTH || y < 0 || y >=VGA_HEIGHT){
        return;
    }
    uint8_t* vga_buffer = (uint8_t*)VGA256_ADDRESS;
    int offset = (y * VGA_WIDTH) + x;
    vga_buffer[offset]=(uint8_t)color;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_get_pixel
 *  Description:  Return the value of the pixel at the given coordinates
 * =====================================================================================
 */
uint8_t M13h_get_pixel(int x, int y){
    if(x < 0 || x>= VGA_WIDTH || y < 0 || y >=VGA_HEIGHT){
        return 0;
    }
    uint8_t* vga_buffer = (uint8_t*)VGA256_ADDRESS;
    int offset = (y * VGA_WIDTH) + x;
    
    return (uint8_t)vga_buffer[offset];
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_clear_screen
 *  Description:  Clear the screen with the given color
 * =====================================================================================
 */
void M13h_clear_screen(int color){
    for(int i=0; i<VGA_HEIGHT;i++){
        for(int j=0; j<VGA_WIDTH;j++){
            M13h_set_pixel(j,i,color);
        }
    }
    current_row=0;
    current_col=0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_put_binary_bitmap
 *  Description:  Take an array containing boolean value, choose color1 or color2 regarding the boolean and put an "image" at the given coordinates
 * =====================================================================================
 */
void M13h_put_binary_bitmap(int x_pos, int y_pos,int width, int height,int color1,int color2, bool arr[]){
    for(int i=0;i<height;i++){
        for(int j=0; j<width; j++){
            if(arr[i*width+j]){
                M13h_set_pixel(x_pos+j,y_pos+i,color1);
            }else{
                M13h_set_pixel(x_pos+j,y_pos+i,color2);
            }
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_put_color_bitmap
 *  Description:  Put an image where each pixel value is stored in an array
 * =====================================================================================
 */
void M13h_put_color_bitmap(int x_pos, int y_pos,int width, int height, uint8_t arr[]){
    for(int i=0;i<height;i++){
        for(int j=0; j<width; j++){
            M13h_set_pixel(x_pos+j,y_pos+i,arr[i*width+j]);
            
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_draw_rectangle
 *  Description:  Draw a rectangle (filed)
 * =====================================================================================
 */
void M13h_draw_rectangle(int x_pos, int y_pos, int width, int height, int color){
    for(int i=0; i<height;i++){
        for(int j=0; j<width;j++){
            M13h_set_pixel(x_pos+j,y_pos+i,color);
        }
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_scroll
 *  Description:  Scroll the full screen and clear the bottom using the bg_color variable
 * =====================================================================================
 */
void M13h_scroll(int nb_line,int bg_color){
    memory_copy(
            (uint8_t * )(VGA256_ADDRESS+nb_line*VGA_WIDTH),
            (uint8_t * )(VGA256_ADDRESS),
            VGA_WIDTH * (VGA_HEIGHT)
    );
    M13h_draw_rectangle(0,VGA_HEIGHT-nb_line,VGA_WIDTH,nb_line,bg_color);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_put_char
 *  Description:  Print a char to screen using the integrated TitanOS font defined in writing/supplier.c
 * =====================================================================================
 */
void M13h_put_char(char ascii_char){
    struct character *graphic_char=get_character(ascii_char);
    if(ascii_char=='\n'){
        if(current_row+10 >= VGA_HEIGHT){
            M13h_scroll(10,background);
        }else{
            current_row+=10;
        }
        current_col=0;
        return;
    }
    if(current_col+graphic_char->graph_width > VGA_WIDTH){
        if((current_row+10 >= VGA_HEIGHT)){
            M13h_scroll(10,background);
        }else{
            current_row+=10;
        }
        current_col=0;
    }
    
    M13h_put_binary_bitmap(current_col,current_row,graphic_char->graph_width,10,foreground,background,graphic_char->graph);
    current_col+=graphic_char->graph_width;
    return;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_print_string
 *  Description:  Print a string
 * =====================================================================================
 */
void M13h_print_string(char* str){
    while(*str){
        M13h_put_char(*str);
        *str++;
    }
    return;
}

void M13h_print_nl(void){
    M13h_put_char('\n');
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  M13h_print_backspace
 *  Description:  Erase a space of 8 bits wide behind the current cursor position but giving the fact that in the used font each character isn't the same width this function is quite useless
 * =====================================================================================
 */
void M13h_print_backspace(void){
    if(current_col <8){
        current_col = 0;
    }else{
        current_col -=8;
    }
    M13h_put_char('\0');
    current_col -=8;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_string
 *  Description:  See which mode the graphics are in and choose the dedicated function
 * =====================================================================================
 */
void print_string(char* str){
    if(is_13h){
        M13h_print_string(str);
    }else{
        M3h_print_string(str);
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_screen
 *  Description:  See which mode the graphics are in and choose the dedicated function
 * =====================================================================================
 */
void clear_screen(uint8_t color){
    if(is_13h){
        M13h_clear_screen(color);
    }else{
        M3h_clear_screen();
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_nl
 *  Description:  See which mode the graphics are in and choose the dedicated function
 * =====================================================================================
 */
void print_nl(void){
    if(is_13h){
        M13h_print_nl();
    }else{
        M3h_print_nl();
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  text_scroll
 *  Description:  See which mode the graphics are in and choose the dedicated function

 * =====================================================================================
 */
void text_scroll(void){
    if(is_13h){
        M13h_scroll(10,background);
    }else{
       int offset = M3h_get_cursor(); 
       offset = M3h_scroll_ln(offset);
       offset = M3h_move_offset_to_new_line(offset);
       M3h_set_cursor(offset);
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  print_backspace
 *  Description:  See which mode the graphics are in and choose the dedicated function
 * =====================================================================================
 */
void print_backspace(void){
    if(is_13h){
        M13h_print_backspace();
    }else{
        M3h_print_backspace();
    }
}
