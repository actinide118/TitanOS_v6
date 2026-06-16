/*
 * =====================================================================================
 *
 *       Filename:  timer.c
 *
 *    Description:  File containing the clock code
 *
 *        Version:  1.0
 *        Created:  15/06/2026 21:14:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "timer.h"
#include "../driver/vga.h"
#include "../util/util.h"
#include "../driver/ports.h"
#include "isr.h"
#include "../kernel.h"
#include "../event/evt_queu.h"

u32 tick = 0;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timer_callback
 *  Description:  Function called after an interruption on the timer IRQ occurs
 * =====================================================================================
 */
static void timer_callback(registers_t *regs) {
    tick++;
    emit_timer_tick(tick);
    /*int offset = M3h_get_cursor();
    M3h_set_cursor((1*80+2)*2);
    M3h_print_string("Tick: ");
    
    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    M3h_print_string(tick_ascii);
    M3h_set_cursor(offset);*/
}
u32 get_tick(){
    return tick;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_timer
 *  Description:  Tell the clock and the processor how to handle time
 * =====================================================================================
 */
void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
