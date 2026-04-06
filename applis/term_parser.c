#include "term_parser.h"
#include "../util/stringmanipulation.h"
#include "../driver/memory.h"

returnstruct_t* parse_command_line(char* command_line){
    if(!is_alphabetic_char(command_line[0])){
        command_parsed_t* err=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
        command_parsed_t** errarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        err->commande="echo";
        err->id=0;
        err->dependencie=0;
        err->dependonhim=0;
        char** arg=(char**)kmalloc(sizeof(char**));
        arg[0]="ERR input must start with an alphabetic character";
        err->args=arg;
        err->argslen=1;
        arr->len=1;
        errarr[0]=err;
        arr->arr=errarr;
        return arr;
    }
}