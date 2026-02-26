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
