#include "keyboard2.h"
#include "../util/ascii.h"
#include <stdint.h>
#include "../event/events.h"
#include "../event/evt_queu.h"
#include "../cpu/isr.h"
#include "ports.h"
#include <stdbool.h>
#include "vga.h"
#include "../util/util.h"
#include "../kernel.h"

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



static struct keymap keymap[] = {
#include "../keymap.my.c"
};
static int shift_mode = 0;
static int alt_mode = 0;
static int ctrl_mode = 0;
static int capslock_mode = 0;
static int numlock_mode = 0;

static void keyboard_interrupt_l2( uint8_t code )
{
	int direction;
	int event;
	if(code > 0x80) {
		direction = 0;
		//print_string("h");
		event = EVENT_KEY_UP;
		code = code & 0x7f;
	} else {
		
		direction = 1;
		event = EVENT_KEY_DOWN;
	}
	struct keymap *k = &keymap[code];

    if(k->special == KEYMAP_SHIFT) {
		shift_mode = direction;
	} else if(k->special == KEYMAP_ALT) {
		alt_mode = direction;
	} else if(k->special == KEYMAP_CTRL) {
		ctrl_mode = direction;
	} else if(k->special == KEYMAP_CAPSLOCK) {
		if(direction == 0) capslock_mode = !capslock_mode;
	} else if(k->special == KEYMAP_NUMLOCK) {
		if(direction == 0) numlock_mode = !numlock_mode;
	}
	uint8_t keymap_flags = 0;
    // Assignation des bits selon les valeurs des variables
    keymap_flags |= (shift_mode ? MASK_SHIFT : 0);
    keymap_flags |= (alt_mode ? MASK_ALT : 0);
    keymap_flags |= (ctrl_mode ? MASK_CTRL : 0);
    keymap_flags |= (capslock_mode ? MASK_CAPSLOCK : 0);
    keymap_flags |= (numlock_mode ? MASK_NUMLOCK : 0);
	if(k->special == KEYMAP_ALPHA){
		keymap_flags |=(KEYMAP_ALPHA ? MASK_ALPHA : 0);
	}
	if(shift_mode == 1 && k->normal == ASCII_BS){
		reset();
	}
	//print_string(&k->normal);
	if (direction == 1){
		emit_keyboard(*k,keymap_flags);
	}else{
		emit_keyboard_up(*k, keymap_flags);
	}

}
static int expect_extra = 0;

static void keyboard_interrupt(registers_t *regs)
{
	uint8_t code = port_byte_in(KEYBOARD_PORT);
	uint8_t c = KEY_INVALID;
	char chaine[3] ;
	//uint8_to_string(code, chaine);
	//print_string(chaine);
	//print_string(" ");

	if(code == KEYCODE_EXTRA) {
		expect_extra = 1;
		return;
	} else if(expect_extra) {
		expect_extra = 0;
		switch(code) {
			case KEYCODE_UP:
				c = KEY_UP;
				break;
			case KEYCODE_DOWN:
				c = KEY_DOWN;
				break;
			case KEYCODE_LEFT:
				c = KEY_LEFT;
				break;
			case KEYCODE_RIGHT:
				c = KEY_RIGHT;
				break;
			default:
				return;
		}
	} else {
		c = code;
	}
	keyboard_interrupt_l2(c);
}
static char key_buffer[256];

void keycallback(struct keymap ch, uint8_t flag){
    if (ch.normal == ASCII_BS) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (ch.normal == ASCII_CR) {
        print_nl();
        char* result[10];
        int count = split(key_buffer, ' ', result, 10);
        execute_command(result);
        key_buffer[0] = '\0';
        print_string("\n> ");
    }else if(ch.special == KEYMAP_ALPHA || ch.special == 0){
		uint8_t shift = (flag & MASK_SHIFT) ? 1 : 0;
    	uint8_t alt = (flag & MASK_ALT) ? 1 : 0;
    	uint8_t ctrl = (flag & MASK_CTRL) ? 1 : 0;
    	uint8_t capslock = (flag & MASK_CAPSLOCK) ? 1 : 0;
    	uint8_t numlock = (flag & MASK_NUMLOCK) ? 1 : 0;

        char letter;

		if(shift == 1){
			letter = ch.shifted;
		}else if(alt == 1){
			letter = ch.alted;
		}else{
			letter = ch.normal;
		}

        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print_string(str);
    }
}
void keyup(struct keymap km, uint8_t arg2){

}
void reset(){
	set_callback_keyboard_up(keyup);
	set_callback_keyboard(keycallback);
	clear_screen();
    cadre_de_couleur(0x20);
    set_cursor((1*80+3)*2);
    print_string_color("hello, TitanOS started",0x9F);
    set_cursor((2*80+3)*2);
	print_string("\n> ");
}

void keyboard_init(){
	set_callback_keyboard_up(keyup);
	set_callback_keyboard(keycallback);
	register_interrupt_handler(IRQ1, keyboard_interrupt);
}