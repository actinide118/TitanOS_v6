
#include <stdint.h>
#include "../gui/text_window.h"
#include "../driver/vga.h"
#include "../driver/memory.h"
#include "../event/evt_queu.h"
#include "../util/ascii.h"
#include "../util/growing_obj.h"

bool Term_init(char *welcome_message);
text_window_t* get_term_window();
growing_obj_t* get_str_buf();
int* get_line_printed();
growing_obj_t* get_user_variables();
char* Get_user_defined_variable(char* key);
void Set_user_defined_variable(char* key,char* value);
void Term_ret(uint8_t code);
void disable_cursor(void);