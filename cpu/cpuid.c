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
#include "../util/util.h"

field_descriptor_t intel_fields[25]={
        {
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=0,
          .end=3,
        },{
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=4,
          .end=7,
        },{
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=8,
          .end=11,
        },{
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=12,
          .end=13,
        },{
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=16,
          .end=19,
        },{
          .reg=0,
          .eax_call=1,
          .ecx_call=0,
          .start=20,
          .end=27,
        },{
          .reg=1,
          .eax_call=1,
          .ecx_call=0,
          .start=0,
          .end=7,
        },{
          .reg=1,
          .eax_call=1,
          .ecx_call=0,
          .start=8,
          .end=15,
        },{
          .reg=1,
          .eax_call=1,
          .ecx_call=0,
          .start=16,
          .end=23,
        },{
          .reg=1,
          .eax_call=1,
          .ecx_call=0,
          .start=24,
          .end=31,
        },{
          .reg=0,
          .eax_call=2,
          .ecx_call=0,
          .start=8,
          .end=15,
        },{
          .reg=0,
          .eax_call=2,
          .ecx_call=0,
          .start=16,
          .end=23,
        },{
          .reg=0,
          .eax_call=2,
          .ecx_call=0,
          .start=24,
          .end=31,
        },{
          .reg=1,
          .eax_call=2,
          .ecx_call=0,
          .start=0,
          .end=7,
        },{
          .reg=1,
          .eax_call=2,
          .ecx_call=0,
          .start=8,
          .end=15,
        },{
          .reg=1,
          .eax_call=2,
          .ecx_call=0,
          .start=16,
          .end=23,
        },{
          .reg=1,
          .eax_call=2,
          .ecx_call=0,
          .start=24,
          .end=31,
        },{
          .reg=2,
          .eax_call=2,
          .ecx_call=0,
          .start=0,
          .end=7,
        },{
          .reg=2,
          .eax_call=2,
          .ecx_call=8,
          .start=15,
          .end=7,
        },{
          .reg=2,
          .eax_call=2,
          .ecx_call=0,
          .start=16,
          .end=23,
        },{
          .reg=2,
          .eax_call=2,
          .ecx_call=0,
          .start=24,
          .end=31,
        },{
          .reg=4,
          .eax_call=2,
          .ecx_call=0,
          .start=0,
          .end=7,
        },{
          .reg=4,
          .eax_call=2,
          .ecx_call=0,
          .start=8,
          .end=15,
        },{
          .reg=4,
          .eax_call=2,
          .ecx_call=0,
          .start=16,
          .end=23,
        },{
          .reg=4,
          .eax_call=2,
          .ecx_call=0,
          .start=24,
          .end=31,
        },
};

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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_vendor_id
 *  Description:  
 * =====================================================================================
 */
uint16_t get_vendor_id ()
{
        char name[15];
        get_vendor_name(name);
        if(strcmp(name,"GenuineIntel")){
          return VENDOR_INTEL;
        }else if(strcmp(name,"AuthenticAMD")){
          return VENDOR_AMD;
        }else{
          return (2<<15)-1;
        }
}		/* -----  end of function get_vendor_id  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_serial_number
 *  Description:  get CPU serial but works only on a few processor because Intel dropped the feature because of privacy concerns
 * =====================================================================================
 */
void get_serial_number (uint32_t* low,uint32_t* high)
{
  cpuid_execute(3,0,&generic_cpuid_var);
  *low=generic_cpuid_var.ecx;
  *high=generic_cpuid_var.edx;
  return;
}		/* -----  end of function get_serial_number  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  check_feature
 *  Description:  Check a feature is present or not use cpuid.h to pass arguments
 * =====================================================================================
 */
bool check_feature (uint16_t feature_flag,uint8_t vendor)
{
  if(vendor!=0){
    return false;
  }
  
  if ( feature_flag > 63 ) {
          return false;
  }
  cpuid_execute(1,0,&generic_cpuid_var);
  if(feature_flag<32){
    if((generic_cpuid_var.ecx & (1<<feature_flag))>0){
      return true;
    }else{
            return false;
    }
  }else{
    if((generic_cpuid_var.edx & (1<<(feature_flag-32)))>0){
      return true;
    }else{
            return false;
    }
  }

  return false;
}		/* -----  end of function check_feature  ----- */



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_intel_field_value
 *  Description:  
 * =====================================================================================
 */
uint32_t get_intel_field_value (uint16_t id)
{
        
        if ( id > 24) {
                return 0;
        }
        cpuid_execute(intel_fields[id].eax_call,intel_fields[id].ecx_call,&generic_cpuid_var);
        uint32_t var;
        switch(intel_fields[id].reg){
          case 0:
                  var=generic_cpuid_var.eax;
                  break;
          case 1:
                  var=generic_cpuid_var.ebx;
                  break;
          case 2:
                  var=generic_cpuid_var.ecx;
                  break;
          case 3:
                  var=generic_cpuid_var.edx;
                  break;
        }
        unsigned width = intel_fields[id].end - intel_fields[id].start + 1u;    
        uint32_t mask = (width == 32u) ? 0xFFFFFFFFu : ((uint32_t)1u << width) - 1u;    
        return (var >> intel_fields[id].start) & mask;
}		/* -----  end of function get_intel_field_value  ----- */

