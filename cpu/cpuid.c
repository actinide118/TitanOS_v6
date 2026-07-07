/*
 * =====================================================================================
 *
 *       Filename:  cpuid.c
 *
 *    Description:  This file contains the methods to identifie the cpu and enumerate features
 *
 *        Version:  1.0
 *        Created:  07/07/2026 02:40:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "cpuid.h"
#include "../driver/vga.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  cpuid_asm
 *  Description:  Execute the assembly cpuid instruction
 * =====================================================================================
 */
static inline void cpuid_asm(uint32_t in_eax, uint32_t in_ecx,uint32_t *out_eax,uint32_t *out_ebx,uint32_t *out_ecx,uint32_t *out_edx){    
  uint32_t eax, ebx, ecx, edx;
  __asm__ volatile (        
    "cpuid"        
    : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)        
    : "a"(in_eax), "c"(in_ecx)        : /* no clobbers needed; cpuid defines a/b/c/d */    
  );
  *out_eax = eax;    *out_ebx = ebx;    *out_ecx = ecx;    *out_edx = edx;
}		/* -----  end of function cpuid_execute  ----- */


/* 
* ===  FUNCTION  ======================================================================
*         Name:  cpuid_execute
*  Description:  Middleware for using the assembly instruction
* =====================================================================================
*/
void cpuid_execute (uint32_t eax_call, uint32_t ecx_call,cpuid_return_t* return_struct)
{
  uint32_t eax_out; 
  uint32_t ebx_out; 
  uint32_t ecx_out; 
  uint32_t edx_out;
  cpuid_asm(0,0,&eax_out,&ebx_out,&ecx_out,&edx_out);
  return_struct->eax=eax_out; 
  return_struct->ebx=ebx_out; 
  return_struct->ecx=ecx_out; 
  return_struct->edx=edx_out;
  return;
}		/* -----  end of function cpuid_execute  ----- */

cpuid_return_t generic_cpuid_var;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_vendor_name
 *  Description:  Use the CPUID.00h call to get the vendor information using the procedure describe in section 21.3 of the volume 1 of the intel Manual
 * =====================================================================================
 */
void get_vendor_name (char* ret_string)
{
  cpuid_execute(0,0,&generic_cpuid_var);
  for (uint8_t i=0 ;i<4 ;i++ ) {
    *ret_string=(char)(generic_cpuid_var.ebx>>i*8);
    ret_string++;
  }
  for (uint8_t i=0 ;i<4 ;i++ ) {
     *ret_string=(char)(generic_cpuid_var.edx>>i*8);
    ret_string++;
  }
  for (uint8_t i=0 ;i<4 ;i++ ) {
     *ret_string=(char)(generic_cpuid_var.ecx>>i*8);
    ret_string++;
  }
  //print_string(generic_cpuid_var.ebx);
  return;
}		/* -----  end of function get_vendor_name  ----- */
