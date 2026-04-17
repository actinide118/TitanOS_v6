#include "term_parser.h"
#include "../util/stringmanipulation.h"
#include "../driver/memory.h"
#include <stdint.h>
#include "terminal.h"
#include "../util/growing_obj.h"
#include "../util/util.h"

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

command_parsed_t* GObj_to_command_parsed(growing_obj_t* obj,uint8_t id){
    int len;
    command_parsed_t* command=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
    len=strlen(obj->value);
    char* copy = kmalloc((len+1)*sizeof(char));
    slice(obj->value,copy,0,len);
    copy[len]='\0';
    command->commande=copy;
    if(obj->next==NULL){
        command->args=NULL;
        command->argslen=0;  
        return command;
    }
    bool isfinished=false;
    uint8_t args_len=0;
    growing_obj_t* curr=obj->next;
    while(!isfinished){
        args_len++;
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
        len=strlen(curr->value);
        char* copy = kmalloc((len+1)*sizeof(char));
        slice(curr->value,copy,0,len);
        copy[len]='\0';
        command->args[i]=copy;
        curr=curr->next;
    }
    return command;
}

returnstruct_t* GObj_to_return_struct(growing_obj_t* obj){
    if(obj->key==NULL){
        returnstruct_t* rt=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        rt->arr=NULL;
        rt->len=0;
        return rt;
    }
    uint8_t obj_len=0;
    bool is_finished=false;
    growing_obj_t* curr = obj;
    while (!is_finished)
    {
        obj_len++;
        if(curr->next != NULL){
            curr = curr->next;
        }else{
            is_finished=true;
        }
    }
    curr=obj;
    command_parsed_t** cmdarr=(command_parsed_t**)kmalloc(obj_len*sizeof(command_parsed_t*));
    returnstruct_t* cmds=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
    for(uint8_t i=0;i<obj_len;i++){
        cmdarr[i]=curr->value;
        curr=curr->next;
    }
    cmds->arr=cmdarr;
    cmds->len=obj_len;
    return cmds;
}

void free_command_parsed(command_parsed_t* command_parsed ){
    //free(command_parsed->commande);
    for(uint8_t i=0;i<command_parsed->argslen;i++){
        //free(command_parsed->args[i]);
    }
    //free(command_parsed->args);
    //free(command_parsed);
}

void free_return_struct(returnstruct_t* parsed_cmd){
    for (uint8_t i=0;i<parsed_cmd->len;i++){
        free_command_parsed(parsed_cmd->arr[i]);
    }
    //free(parsed_cmd->arr);
    //free(parsed_cmd);
}


returnstruct_t* parsing_error(char* str){
        command_parsed_t* err=(command_parsed_t*)kmalloc(sizeof(command_parsed_t));
        command_parsed_t** errarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        err->commande="echolor";
        err->id=0;
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
        char** arg=(char**)kmalloc(sizeof(char**)*3);
        arg[0]="ERR: input must start with an alphabetic character\n";
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
                return parsing_error("ERR: string is not finished \n");
            }
            isfinished=true;
            if(command_line[index-1]!=' '){
            
                uint32_t len=index-index_since_block_start;
                char* strbuf=kmalloc((len+1)*sizeof(char));
                slice(command_line,strbuf,index_since_block_start,len);
                strbuf[len]='\0';
                char* buf=kmalloc(3*sizeof(char));
                uint8_to_string(index_curr_cmd_arr,buf);
                GObj_set(curr_cmd,buf,strbuf);
            }
            char* strbuf=(char*)kmalloc(3*sizeof(char));
            uint8_to_string(index_command_arr,strbuf);
            command_parsed_t* cmd=GObj_to_command_parsed(curr_cmd,0);
            uint8_to_string(index_command_arr,strbuf);
            GObj_set(commandes,strbuf,cmd);
            //GObj_clear(curr_cmd);
            
            index_curr_cmd_arr++;
        }
        if(command_line[index]=='"'){
            //WText_printstring(get_term_window(),"\" encounter\n");
            if(is_single_quoted){
                index++;
                continue;
            }
            if(!(is_double_quoted||is_single_quoted)&&(command_line[index-1]!=' '&&command_line[index-1]!='=')){
               return parsing_error("ERR quote must be used after a space or =\n");
            }
            is_double_quoted=!is_double_quoted;
        }else if(command_line[index]=='\''){
            if(is_double_quoted){
                index++;
                continue;
            }
            if(!(is_double_quoted||is_single_quoted)&&command_line[index-1]!=' '){
               return parsing_error("ERR quote must be used after a space\n");
            }
            is_single_quoted=!is_single_quoted;
        }else if(command_line[index]==' '){
            if(is_double_quoted||is_single_quoted){
                index++;
                continue;
            }
            if(command_line[index-1]==' '){
                index++;
                index_since_block_start++;
                continue;
            }

            uint32_t len=index-index_since_block_start;
            char* strbuf=kmalloc((len+1)*sizeof(char));
            slice(command_line,strbuf,index_since_block_start,len);
            strbuf[len]='\0';
            char* buf=kmalloc(3*sizeof(char));
            uint8_to_string(index_curr_cmd_arr,buf);
            GObj_set(curr_cmd,buf,strbuf);
            
            index_curr_cmd_arr++;
            index_since_block_start=index+1;
        }else if(command_line[index]=='&'){
            if(is_double_quoted||is_single_quoted){
                index++;
                continue;
            }
            if(command_line[index+1]=='&'){
                if(command_line[index-1]!=' '){
                    return parsing_error("ERR: && must be preceeded by a space\n");
                }
                if(command_line[index+2]!=' '){
                    return parsing_error("ERR: && must be followed by a space\n");
                }
                command_parsed_t* cmd=GObj_to_command_parsed(curr_cmd,0);
                char* strbuf=(char*)kmalloc(3*sizeof(char));
                uint8_to_string(index_command_arr,strbuf);
                GObj_set(commandes,strbuf,cmd);
                //GObj_clear(curr_cmd);
                curr_cmd=GObj_create();
                index_curr_cmd_arr=0;
                index_command_arr++;
                index+=3;
                index_since_block_start+=3;
                continue;
            }
        }
        index++;
    }
    /*printGObj(commandes);
    WText_printstring(get_term_window(),"\n>");
        returnstruct_t* arr=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
        arr->len=0;
        return arr;
    
    command_parsed_t* cmd=GObj_to_command_parsed(curr_cmd,0);
    command_parsed_t** cmdarr=(command_parsed_t**)kmalloc(sizeof(command_parsed_t*));
    returnstruct_t* cmds=(returnstruct_t*)kmalloc(sizeof(returnstruct_t));
    cmdarr[0]=cmd;
    cmds->arr=cmdarr;
    cmds->len=1;
    return cmds;
    */
   returnstruct_t* rt=GObj_to_return_struct(commandes);
   //GObj_free(commandes);
   return rt;
}