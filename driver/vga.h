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

void print_string(char *string);

void print_string_color(char *string, int color);

void print_nl();

void clear_screen();

int scroll_ln(int offset);

void set_cursor(int offset);

int get_cursor();

void error_occured(char *string);

void print_backspace();

void set_char_at_video_memory(char caracter, int offset);

void cadre_de_couleur(int color);

void set_char_colr_at_video_memory(char ch,int offset, int color);