/*
 * =====================================================================================
 *
 *       Filename:  stringmanipulation.c
 *
 *    Description:  File containing functions to manipulate strings 
 *
 *        Version:  1.0
 *        Created:  27/05/2026 21:22:21
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "stringmanipulation.h"
#include <stdint.h>

void to_upper_case(char* str){
    while(*str){
        if(*str>=0x61 && *str<=0x7A){
            *str=*str-32;
        }
        str++;
    }
}

void to_lower_case(char* str){
    while(*str){
        if(*str>=0x41 && *str<=0x5A){
            *str=*str+32;
        }
        str++;
    }
}

bool is_alphabetic_char(char ch){
    if((ch>=0x41 && ch<=0x5A)||(ch>=0x61 && ch<=0x7A)){
        return true;
    }else{
        return false;
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  slice
 *  Description:  Copy a part of a string in another string ( offer no protection against buffer overflow ) 
 * =====================================================================================
 */
void slice(char *strmere,char *strfille,uint32_t index,uint32_t len){
    for(uint32_t i=0;i<len;i++){
        strfille[i]=strmere[index+i];
    }
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  indexOf
 *  Description:  Return the first occurence of a given char in a string or -1 if it can't find any 
 * =====================================================================================
 */
int indexOf(char* str,char ch){
    int index=0;
    while (*str)
    {
        if(*str==ch){
            return index;
        }
        index++;
        *str++;
    }
    return -1;
}
