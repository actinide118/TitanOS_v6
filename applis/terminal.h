
#include <stdint.h>
#include "../gui/text_window.h"
#include "../driver/vga.h"
#include "../driver/memory.h"
#include "../event/evt_queu.h"
#include "../util/ascii.h"

bool Term_init(char *welcome_message);
text_window_t* get_term_window();