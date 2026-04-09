#include "term_parser.h"
#include "../util/stringmanipulation.h"
#include "../driver/memory.h"
#include <stdint.h>
#include "terminal.h"
#include "../util/growing_obj.h"
#include "../util/util.h"

command_parsed_t* GObj_to_command_parsed(growing_obj_t* obj,uint8_t id,uint8_t after,uint8_t before){
    command_parsed_t* command=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
    command->commande=obj->value;
    command->dependencie=after;
    command->dependonhim=before;
    if(obj->next==NULL){
        command->args=NULL;
        command->argslen=0;  
    }
    bool isfinished=false;
    uint8_t args_len=0;
    growing_obj_t* curr=obj->next;
    while(!isfinished){
        args_len++;
        //WText_printstring(get_term_window(),"f");
        if(curr->next==NULL){
            isfinished=true;
            break;
        }
        curr=curr->next;
    }
    curr=obj->next;
    command->args=(char**)kmalloc(args_len*sizeof(char*));
    command->argslen=args_len;
    for(uint8_t i=0;i<args_len;i++){
        command->args[i]=curr->value;
        curr=curr->next;
    }
    return command;
}

void printGObj(growing_obj_t* obj){
    growing_obj_t* curr=obj;
    bool is_finished=false;
    while(!is_finished){
        WText_printstring(get_term_window(),curr->key);
        WText_printstring(get_term_window(),"=>");
        WText_printstring(get_term_window(),(char*)curr->value);
        WText_printstring(get_term_window(),"\n");
        if(curr->next==NULL){
            is_finished=true;
        }else{
            curr=curr->next;
        }
    }
}

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
            if(command_line[index-1]=='"'||command_line[index-1]=='\''){
               uint32_t len=index-index_since_block_start-2;
                char* strbuf=kmalloc((len+1)*sizeof(char));
                slice(command_line,strbuf,index_since_block_start+1,len);
                char* buf=kmalloc(3*sizeof(char));
                uint8_to_string(index_curr_cmd_arr,buf);
                GObj_set(curr_cmd,buf,strbuf); 
            }else{
                uint32_t len=index-index_since_block_start;
                char* strbuf=kmalloc((len+1)*sizeof(char));
                slice(command_line,strbuf,index_since_block_start,len);
                char* buf=kmalloc(3*sizeof(char));
                uint8_to_string(index_curr_cmd_arr,buf);
                GObj_set(curr_cmd,buf,strbuf);
            }
            index_curr_cmd_arr++;
            index_since_block_start=index+1;
        }
        if(command_line[index]=='"'){
            if(is_single_quoted){
                index++;
                continue;
            }
            if(!(is_double_quoted||is_single_quoted)&&command_line[index-1]!=' '){
               return parsing_error("ERR quote must be used after a space");
            }
            is_double_quoted=!is_double_quoted;
        }else if(command_line[index]=='\''){
            if(is_double_quoted){
                index++;
                continue;
            }
            if(!(is_double_quoted||is_single_quoted)&&command_line[index-1]!=' '){
               return parsing_error("ERR quote must be used after a space");
            }
            is_single_quoted=!is_single_quoted;
        }else if(command_line[index]==' '){
            if(is_double_quoted||is_single_quoted){
                index++;
                continue;
            }
            if(command_line[index-1]=='"'||command_line[index-1]=='\''){
               uint32_t len=index-index_since_block_start-2;
                char* strbuf=kmalloc((len+1)*sizeof(char));
                slice(command_line,strbuf,index_since_block_start+1,len);
                char* buf=kmalloc(3*sizeof(char));
                uint8_to_string(index_curr_cmd_arr,buf);
                GObj_set(curr_cmd,buf,strbuf); 
            }else{
                uint32_t len=index-index_since_block_start;
                char* strbuf=kmalloc((len+1)*sizeof(char));
                slice(command_line,strbuf,index_since_block_start,len);
                char* buf=kmalloc(3*sizeof(char));
                uint8_to_string(index_curr_cmd_arr,buf);
                GObj_set(curr_cmd,buf,strbuf);
            }
            index_curr_cmd_arr++;
            index_since_block_start=index+1;
        }
        index++;
    }
    //printGObj(curr_cmd);
    command_parsed_t* cmd=GObj_to_command_parsed(curr_cmd,0,0,0);
    command_parsed_t** cmdarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
    returnstruct_t* cmds=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
    cmdarr[0]=cmd;
    cmds->arr=cmdarr;
    cmds->len=1;
    return cmds;
}