#include "term_parser.h"
#include "../util/stringmanipulation.h"
#include "../driver/memory.h"
#include <stdint.h>
#include "terminal.h"
#include "../util/growing_obj.h"

returnstruct_t* parsing_error(char* str){
        command_parsed_t* err=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
        command_parsed_t** errarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        err->commande="echolor";
        err->id=0;
        err->dependencie=0;
        err->dependonhim=0;
        char** arg=(char**)kmalloc(sizeof(char**)*3);
        arg[0]=str;
        arg[1]="4";
        arg[2]="0";
        err->args=arg;
        err->argslen=3;
        arr->len=1;
        errarr[0]=err;
        arr->arr=errarr;
        return arr;
}

returnstruct_t* parse_command_line(char* command_line){
    if(command_line[0]=='\0'){
        WText_printstring(get_term_window(),"\n>");
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        arr->len=0;
        return arr;
    }
    if(!is_alphabetic_char(command_line[0])){
        command_parsed_t* err=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
        command_parsed_t** errarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        err->commande="echolor";
        err->id=0;
        err->dependencie=0;
        err->dependonhim=0;
        char** arg=(char**)kmalloc(sizeof(char**)*3);
        arg[0]="ERR input must start with an alphabetic character";
        arg[1]="4";
        arg[2]="0";
        err->args=arg;
        err->argslen=3;
        arr->len=1;
        errarr[0]=err;
        arr->arr=errarr;
        return arr;
    }
    bool isfinished=false;
    bool is_single_quoted=false;
    bool is_double_quoted=false;
    uint8_t index_command_arr=0;
    uint8_t index_curr_cmd_arr=0;
    uint8_t index_since_block_start=0;
    uint8_t nb_block_in_command=0;
    uint8_t index=0;
    growing_obj_t* commandes=GObj_create();
    growing_obj_t* curr_cmd=GObj_create();
    GObj_set(commandes,"0",curr_cmd);
    while(!isfinished){
        if(command_line[index]=='\0'){
            if(is_single_quoted||is_double_quoted){
                isfinished=true;
                command_parsed_t* err=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
                command_parsed_t** errarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
                returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
                err->commande="echolor";
                err->id=0;
                err->dependencie=0;
                err->dependonhim=0;
                char** arg=(char**)kmalloc(sizeof(char**)*3);
                arg[0]="ERR string not closed";
                arg[1]="4";
                arg[2]="0";
                err->args=arg;
                err->argslen=3;
                arr->len=1;
                errarr[0]=err;
                arr->arr=errarr;
                return arr;
            }
            isfinished=true;
        }
        if(command_line[index]=='"'){
            if(is_single_quoted){
                break;
            }
            if(!(is_double_quoted||is_single_quoted)&&command_line[index-1]!=' '){
               return parsing_error("ERR quote must be used after a space");
            }
            is_double_quoted=!is_double_quoted;
        }else if(command_line[index]=='\''){
            if(is_double_quoted){
                break;
            }
            if(!(is_double_quoted||is_single_quoted)&&command_line[index-1]!=' '){
               return parsing_error("ERR quote must be used after a space");
            }
            is_single_quoted=!is_single_quoted;
        }else if(command_line[index]==' '){
            uint32_t len=index-index_since_block_start;
            char* strbuf=kmalloc(len*sizeof(char));
            slice(command_line,strbuf,index,len);
            //GObj_get(curr_cmd)
        }
        index++;
    }
}