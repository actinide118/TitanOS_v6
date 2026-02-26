#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

void memory_copy(char *source, char *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);
void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
bool backspace(char s[]);
void append(char s[], char n);
void hex_to_ascii(int n, char str[]);
int strcmp(char s1[], char s2[]) ;
void uint16_to_hex(uint16_t value, char* buffer);
int split(const char* str, char delimiter, char* result[], int max_splits);
uint32_t string_to_uint32(const char* str);
void uint32_to_string(uint32_t num, char* buffer);
void uint16_array_to_string(uint16_t* array, size_t length, char* result_buffer);
u32 kmalloc(u32 size, int align, u32 *phys_addr);
uint8_t string_to_uint8(const char* str);


#endif