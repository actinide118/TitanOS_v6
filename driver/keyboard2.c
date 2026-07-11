/*
 * =====================================================================================
 *
 *       Filename:  keyboard2.c
 *
 *    Description:  File containing how TitanOS handle keyboard interrupts using a PS/2 keyboard
 *
 *        Version:  1.0
 *        Created:  14/06/2026 21:18:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
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
#include "../driver/memory.h"




static struct keymap keymap[] = {
#include "../keymap.my.c"
};
static int shift_mode = 0;
static int alt_mode = 0;
static int ctrl_mode = 0;
static int capslock_mode = 0;
static int numlock_mode = 0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keyboard_interrupt_l2
 *  Description:  Second layer it interpret the code to find which key was pressed using the keymap defined in keymap.my.c in the root directory and emit the keyboard or keyboard_up event
 * =====================================================================================
 */
static void keyboard_interrupt_l2( uint8_t code )
{
	int direction;
	int event;
	if(code > 0x80) {
		direction = 0;
		//M3h_print_string("h");
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
	//M3h_print_string(&k->normal);
	if (direction == 1){
		emit_keyboard(*k,keymap_flags);
	}else{
		emit_keyboard_up(*k, keymap_flags);
	}

}
static int expect_extra = 0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keyboard_interrupt
 *  Description:  First layer of the interrupt process. It is the function called by the interrupt table, it read the input and call the second layer
 * =====================================================================================
 */
static void keyboard_interrupt(registers_t *regs)
{
	uint8_t code = port_byte_in(KEYBOARD_PORT);
	uint8_t c = KEY_INVALID;
	char chaine[3] ;
	//uint8_to_string(code, chaine);
	//M3h_print_string(chaine);
	//M3h_print_string(" ");

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
char* key_buffer;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keycallback
 *  Description:  Default handler of the keyboard event it add the character to the command line of the basic terminal (the first when you boot)
 * =====================================================================================
 */
void keycallback(struct keymap ch, uint8_t flag){
    if (ch.normal == ASCII_BS) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
    } else if (ch.normal == ASCII_CR) {
        print_nl();
        char* result[10];
        int count = split(key_buffer, ' ', result, 10);
        bool disable = execute_command(result);
        key_buffer[0] = '\0';
		if(!disable){
        	print_string("\n> ");
		}
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keyup
 *  Description:  Dummy handler for the keyboard_up event
 * =====================================================================================
 */
void keyup(struct keymap km, uint8_t arg2){

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  reset
 *  Description:  Reset the keyboard's function handler and clear the screen to show the basic terminal
 * =====================================================================================
 */
void reset(){
	set_callback_keyboard_up(keyup);
	set_callback_keyboard(keycallback);
	if(Is_13h()){
		clear_screen(get_background());
		M13h_print_string("\n> ");

		return;
	}
	M3h_clear_screen();
    M3h_cadre_de_couleur(0x20);
    M3h_set_cursor((1*80+3)*2);
    M3h_print_string_color("hello, TitanOS started",0x9F);
    M3h_set_cursor((2*80+3)*2);
	M3h_print_string("\n> ");
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  keyboard_init
 *  Description:  It set up the handlers and register the interrupt in the interrupt table
 * =====================================================================================
 */
void keyboard_init(){
	set_callback_keyboard_up(keyup);
	set_callback_keyboard(keycallback);
	register_interrupt_handler(IRQ1, keyboard_interrupt);
  key_buffer=kmalloc(256);
}
