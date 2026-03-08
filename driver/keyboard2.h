#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>

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