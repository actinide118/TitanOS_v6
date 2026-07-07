#include <stdint.h>

typedef struct rreturn{
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
}cpuid_return_t;

void cpuid_execute (uint32_t eax_call, uint32_t ecx_call,cpuid_return_t* return_struct);
void get_vendor_name(char* ret_sting);

