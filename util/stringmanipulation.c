#include "stringmanipulation.h"

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