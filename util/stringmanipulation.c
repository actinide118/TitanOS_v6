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

void slice(char *strmere,char *strfille,uint32_t index,uint32_t len){
    for(uint32_t i=0;i<len;i++){
        strfille[i]=strmere[index+i];
    }
}