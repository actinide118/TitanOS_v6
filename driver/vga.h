#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define RED_ON_BLACK 0x04
#define BLACK_ON_RED 0x40
#define VGA256_ADDRESS 0xA0000
#define VGA_WIDTH 320
#define VGA_HEIGHT 200

#include <stdint.h>

void M3h_print_string(char *string);

void M3h_print_string_color(char *string, int color);

void M3h_print_nl();

void M3h_clear_screen();

int M3h_scroll_ln(int offset);

void M3h_set_cursor(int offset);

int M3h_get_cursor();

void error_occured(char *string);

void M3h_print_backspace();

void M3h_set_char_at_video_memory(char caracter, int offset);

void M3h_cadre_de_couleur(int color);

void M3h_set_char_colr_at_video_memory(char ch,int offset, int color);

bool Is_13h(void);

uint8_t get_foreground(void);

uint8_t get_background(void);

void set_foreground(uint8_t color);

void set_background(uint8_t color);

void switch_to_13h(void);

void M13h_clear_screen(int color);

void M13h_set_pixel(int x, int y, int color);

void M13h_put_binary_bitmap(int x_pos, int y_pos,int width, int height,int color1,int color2, bool arr[]);

void M13h_put_color_bitmap(int x_pos, int y_pos,int width, int height, uint8_t arr[]);

void M13h_draw_rectangle(int x_pos, int y_pos, int width, int height, int color);

void M13h_scroll(int nb_line,int bg_color);

void M13h_put_char(char ascii_char);

void M13h_print_string(char* string);

void M13h_print_nl(void);

void M13h_print_backspace(void);

void print_string(char* str);

void clear_screen(uint8_t color);

void print_nl(void);

void text_scroll(void);

void print_backspace(void);