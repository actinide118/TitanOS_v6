/*
 * =====================================================================================
 *
 *       Filename:  features.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/09/2026 08:57:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "cpuid.h"
#include "features.h"
#include "../util/util.h"

char* sse3="SSE3";
char* pcmulqdq="PCMULQDQ";
char* dtes64="DTES64";
char* monitor="MONITOR";
char* ds_cpl="DS_CPL";
char* vmx="VMX";
char* smx="SMX";
char* eist="EIST";
char* tm2="TM2";
char* ssse3="SSSE3";
char* l1_context_id="L1_CONTEXT_ID";
char* debug_interface="DEBUG_INTERFACE";
char* fma="FMA";
char* cmpxchg16b="CMPXCHG16B";
char* xtpr_update_control="XTPR_UC";
char* perf_capabilities="PERF_C";
char* pcid="PCID";
char* dca="DCA";
char* sse4_1="SSE4_1";
char* sse4_2="SSE4_2";
char* x2apic="X2APIC";
char* movbe="MOVBE";
char* popcnt="POPCNT";
char* tsc_deadline="TSC_D";
char* aesni="AESNI";
char* xsave="XSAVE";
char* osxsave="OSXSAVE";
char* avx="AVX";
char* f16c="F16C";
char* rdrand="RDRAND";
char* on_chip_fpu="FPU";
char* vme="VME";
char* de="DE";
char* pse="PSE";
char* tsc="TSC";
char* msr="MSR";
char* pae="PAE";
char* mce="MCE";
char* cmpxchg8b="CMPXCHG8B";
char* apic="APIC";
char* sep="SEP";
char* mtrr="MTRR";
char* pge="PGE";
char* mca="MCA";
char* cmov="CMOV";
char* pat="PAT";
char* pse36="PSE36";
char* psn="PSN";
char* clflush="CLFLUSH";
char* ds="DS";
char* acpi="ACPI";
char* mmx="MMX";
char* fxsr="FXSR";
char* sse="SSE";
char* sse2="SSE2";
char* self_snoop="SELF_SNOOP";
char* htt="HTT";
char* tm="TM";
char* pbe="PBE";
char* rsv="";

char** intel_features[64]={
&sse3,&pcmulqdq,&dtes64,&monitor,&ds_cpl,&vmx,&smx,
&eist,&tm2,&ssse3,&l1_context_id,&debug_interface,
&fma,&cmpxchg16b,&xtpr_update_control,&perf_capabilities,
&rsv,&pcid,&dca,&sse4_1,&sse4_2,&x2apic,&movbe,&popcnt,
&tsc_deadline,&aesni,&xsave,&osxsave,&avx,&f16c,&rdrand,
&rsv,&on_chip_fpu,&vme,&de,&pse,&tsc,&msr,&pae,&mce,
&cmpxchg8b,&apic,&rsv,&mtrr,&pge,&mca,&cmov,&pat,&pse36,
&psn,&clflush,&rsv,&ds,&acpi,&mmx,&fxsr,&sse,&sse2,
&self_snoop,&htt,&tm,&rsv,&pbe
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_intel_features_name
 *  Description:  
 * =====================================================================================
 */
char* get_intel_features_name (uint16_t feature)
{
        return *intel_features[feature];
}		/* -----  end of function get_intel_features  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_intel_index
 *  Description:  
 * =====================================================================================
 */
uint16_t get_intel_index (char* id)
{
        uint16_t index=0;
        while(index<63){
                
                if ( strcmp(id,*intel_features[index])==0 ) {
                        return index;
                }
                index++;
        }
        return (2<<15)-1;
}		/* -----  end of function get_intel_index  ----- */




