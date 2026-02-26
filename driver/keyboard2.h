#ifndef KEYBOARD_H
#define KEYBOARD_H

struct keymap {
	char normal;
	char shifted;
	char alted;
	char special;
};
void keyboard_init();
void reset();

#endif