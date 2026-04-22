#ifndef KERNEL_H
#define KERNEL_H

bool execute_command(char* input[]);
uint32_t get_seed(void);
void set_seed(uint32_t value);

#endif