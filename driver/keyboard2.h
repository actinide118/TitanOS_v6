#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

#define KEYBOARD_PORT 0x60

#define KEYMAP_SHIFT 1
#define KEYMAP_ALT   2
#define KEYMAP_CTRL  3
#define KEYMAP_CAPSLOCK 4
#define KEYMAP_NUMLOCK 5
#define KEYMAP_ALPHA 6
#define KEYMAP_NUMPAD 8

#define MASK_SHIFT     (1 << 0) // Bit 0
#define MASK_ALT       (1 << 1) // Bit 1
#define MASK_CTRL      (1 << 2) // Bit 2
#define MASK_CAPSLOCK  (1 << 3) // Bit 3
#define MASK_NUMLOCK   (1 << 4)
#define MASK_ALPHA     (1 << 5)

/* sent before certain keys such as up, down, left, or right. */
#define KEYCODE_EXTRA (uint8_t)0xE0
#define KEYCODE_UP    (uint8_t)0x48
#define KEYCODE_DOWN  (uint8_t)0x42
#define KEYCODE_LEFT  (uint8_t)0x4B
#define KEYCODE_RIGHT (uint8_t)0x4D

#define BUFFER_SIZE 256

struct keymap {
	char normal;
	char shifted;
	char alted;
	char special;
};
void keyboard_init();
void reset();
void keycallback(struct keymap ch, uint8_t flag);

#endif