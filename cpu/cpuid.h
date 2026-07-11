#include <stdint.h>

#define VENDOR_INTEL 0
#define VENDOR_AMD 1
//This first features are enumerated in CPUID.01h:ECX
#define INTEL_SSE3 0
#define INTEL_PCMULQDQ 1
#define INTEL_DTES64 2
#define INTEL_MONITOR 3
#define INTEL_DS_CPL 4
#define INTEL_VMX 5
#define INTEL_SMX 6
#define INTEL_EIST 7
#define INTEL_TM2 8
#define INTEL_SSSE3 9
#define INTEL_L1_CONTEXT_ID 10
#define INTEL_DEBUG_INTERFACE 11
#define INTEL_FMA 12
#define INTEL_CMPXCHG16B 13
#define INTEL_XTPR_UPDATE_CONTROL 14
#define INTEL_PERF_CAPABILITIES 15
//16 reserved
#define INTEL_PCID 17
#define INTEL_DCA 18
#define INTEL_SSE4_1 19
#define INTEL_SSE4_2 20
#define INTEL_X2APIC 21
#define INTEL_MOVBE 22
#define INTEL_POPCNT 23
#define INTEL_TSC_DEADLINE 24
#define INTEL_AESNI 25
#define INTEL_XSAVE 26
#define INTEL_OSXSAVE 27
#define INTEL_AVX 28
#define INTEL_F16C 29
#define INTEL_RDRAND 30
//bit 31 not used
//This features are enumerated in CPUID.01h:EDX
#define INTEL_ON_CHIP_FPU 32+0
#define INTEL_VME 32+1
#define INTEL_DE 32+2
#define INTEL_PSE 32+3
#define INTEL_TSC 32+4
#define INTEL_MSR 32+5
#define INTEL_PAE 32+6
#define INTEL_MCE 32+7
#define INTEL_CMPXCHG8B 32+8
#define INTEL_APIC 32+9
//bit 10 reserved
#define INTEL_SEP 32+11
#define INTEL_MTRR 32+12
#define INTEL_PGE 32+13
#define INTEL_MCA 32+14
#define INTEL_CMOV 32+15
#define INTEL_PAT 32+16
#define INTEL_PSE36 32+17
#define INTEL_PSN 32+18
#define INTEL_CLFLUSH 32+19
//bit 20 reserved
#define INTEL_DS 32+21
#define INTEL_ACPI 32+22
#define INTEL_MMX 32+23
#define INTEL_FXSR 32+24
#define INTEL_SSE 32+25
#define INTEL_SSE2 32+26
#define INTEL_SELF_SNOOP 32+27
#define INTEL_HTT 32+28
#define INTEL_TM 32+29
//bit 30 reserved
#define INTEL_PBE 32+31

//Fields that are not bit but value
#define INTEL_NUMBER_STEPPING_ID 0
#define INTEL_NUMBER_MODEL_ID 1
#define INTEL_NUMBER_FAMILY_ID 2
#define INTEL_NUMBER_PROCESSOR_TYPE 3
#define INTEL_NUMBER_EXTENDED_MODEL_ID 4
#define INTEL_NUMBER_EXTENDED_FAMILY_ID 5
#define INTEL_NUMBER_BRAND_INDEX 6
#define INTEL_NUMBER_CLFLUSH_LINE_SIZE 7
#define INTEL_NUMBER_APIC_ID_SPACE 8
#define INTEL_NUMBER_INITIAL_APIC_ID 9
#define INTEL_NUMBER_DESCRIPTOR_1 10
#define INTEL_NUMBER_DESCRIPTOR_2 11
#define INTEL_NUMBER_DESCRIPTOR_3 12
#define INTEL_NUMBER_DESCRIPTOR_4 13
#define INTEL_NUMBER_DESCRIPTOR_5 14
#define INTEL_NUMBER_DESCRIPTOR_6 15
#define INTEL_NUMBER_DESCRIPTOR_7 16
#define INTEL_NUMBER_DESCRIPTOR_8 17
#define INTEL_NUMBER_DESCRIPTOR_9 18
#define INTEL_NUMBER_DESCRIPTOR_10 19
#define INTEL_NUMBER_DESCRIPTOR_11 20
#define INTEL_NUMBER_DESCRIPTOR_12 21
#define INTEL_NUMBER_DESCRIPTOR_13 22
#define INTEL_NUMBER_DESCRIPTOR_14 23
#define INTEL_NUMBER_DESCRIPTOR_15 24

typedef struct d{
  uint8_t reg; //0:EAX 1:EBX 2:ECX 3:EDX
  uint16_t eax_call;
  uint16_t ecx_call;
  uint8_t start;
  uint8_t end;
}field_descriptor_t;

typedef struct rreturn{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
}cpuid_return_t;

void cpuid_execute (uint32_t eax_call, uint32_t ecx_call,cpuid_return_t* return_struct);
void get_vendor_name(char* ret_sting);
void get_serial_number (uint32_t* low,uint32_t* high);
bool check_feature (uint16_t feature_flag,uint8_t vendor);
uint32_t get_intel_field_value (uint16_t id);


