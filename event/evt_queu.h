#include "../driver/keyboard2.h"
typedef void (*callback_t)(struct keymap, uint8_t);
typedef void (*callback_tick_t)(uint32_t);

void set_callback_keyboard(callback_t cb);
void emit_keyboard(struct keymap km, uint8_t arg2);
void set_callback_keyboard_up(callback_t cb);
void emit_keyboard_up(struct keymap km, uint8_t arg2);
void set_callback_tick(callback_tick_t cb);
void emit_timer_tick(uint32_t nb);