#include <stdint.h>
#include "vga.h"
#include "ports.h"
#include "../util/util.h"


void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_byte_in(VGA_DATA_REGISTER);
    return offset * 2;
}
void set_char_at_video_memory(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}
int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset) {
    return get_offset(2, get_row_from_offset(offset) + 1);
}

int scroll_ln(int offset) {
    memory_copy(
            (uint8_t * )(get_offset(0, 1) + VIDEO_ADDRESS),
            (uint8_t * )(get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}


void print_string(char *string) {
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_at_video_memory(string[i], offset);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}
void set_char_colr_at_video_memory(char ch,int offset, int color){
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = ch;
    vidmem[offset + 1] = color;
}
void print_string_color(char *string, int color){
    int offset = get_cursor();
    int i = 0;
    while (string[i] != 0) {
        if (offset >= MAX_ROWS * MAX_COLS * 2) {
            offset = scroll_ln(offset);
        }
        if (string[i] == '\n') {
            offset = move_offset_to_new_line(offset);
        } else {
            set_char_colr_at_video_memory(string[i], offset, color);
            offset += 2;
        }
        i++;
    }
    set_cursor(offset);
}

void print_nl() {
    int newOffset = move_offset_to_new_line(get_cursor());
    if (newOffset >= MAX_ROWS * MAX_COLS * 2) {
        newOffset = scroll_ln(newOffset);
    }
    set_cursor(newOffset);
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    for (int i = 0; i < screen_size; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void print_backspace() {
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}
void cadre_de_couleur(int color){
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
void error_occured(char *string){
    clear_screen();
    cadre_de_couleur(BLACK_ON_RED);
    int offset = (10*80+30)*2;
    int i = 0;
    while (string[i] != 0) {
        unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
        vidmem[offset] = string[i];
        vidmem[offset + 1] = RED_ON_BLACK;
        offset += 2;
        i++;
    }
    set_cursor((10*80+30+i)*2);
}


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

}

void M13h_set_pixel(int x, int y, int color){
    if(x < 0 || x>= VGA_WIDTH || y < 0 || y >=VGA_HEIGHT){
        return;
    }
    uint8_t* vga_buffer = (uint8_t*)VGA256_ADDRESS;
    int offset = (y * VGA_WIDTH) + x;
    vga_buffer[offset]=(uint8_t)color;
}

void M13h_clear_screen(int color){
    for(int i=0; i<VGA_HEIGHT;i++){
        for(int j=0; j<VGA_WIDTH;j++){
            M13h_set_pixel(j,i,color);
        }
    }
}

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
void M13h_put_color_bitmap(int x_pos, int y_pos,int width, int height, uint8_t arr[]){
    for(int i=0;i<height;i++){
        for(int j=0; j<width; j++){
            M13h_set_pixel(x_pos+j,y_pos+i,arr[i*width+j]);
            
        }
    }
}
void M13h_draw_rectangle(int x_pos, int y_pos, int width, int height, int color){
    for(int i=0; i<height;i++){
        for(int j=0; j<width;j++){
            M13h_set_pixel(x_pos+j,y_pos+i,color);
        }
    }
}
void M13h_scroll(int nb_line,int bg_color){
    memory_copy(
            (uint8_t * )(VGA256_ADDRESS+nb_line*VGA_WIDTH),
            (uint8_t * )(VGA256_ADDRESS),
            VGA_WIDTH * (VGA_HEIGHT)
    );
    M13h_draw_rectangle(0,VGA_HEIGHT-nb_line,VGA_WIDTH,nb_line,bg_color);
}