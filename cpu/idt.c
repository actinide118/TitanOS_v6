/*
 * =====================================================================================
 *
 *       Filename:  idt.c
 *
 *    Description:  File containing the methods to interract with the interrupt descriptor table
 *
 *        Version:  1.0
 *        Created:  15/06/2026 20:41:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "idt.h"
#include "../util/util.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_idt_gate
 *  Description:  Modify an entry of the IDT to point with a new handler
 * =====================================================================================
 */
void set_idt_gate(int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_16(handler);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  set_idt
 *  Description:  Tell the processor where in the memory the IDT is
 * =====================================================================================
 */
void set_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
