#include "terminal_executor.h"
#include "../util/util.h"
#include "../util/stringmanipulation.h"
#include "../snake/main.h"
#include "../2048/main2048.h"

char* prepstring(char* str){
    int len=0;
    char* cp=str;
    while(*cp){
        len++;
        *cp++;
    }
    /*WText_printstring_color(get_term_window(),str,3,0);
    WText_printstring(get_term_window(),"prepstring\n");*/
    if(str[0]=='$'){
        cp=kmalloc(len*sizeof(char));
        slice(str,cp,1,len-1);
        return prepstring(Get_user_defined_variable(cp));
    }
    int cpindex=0;
    int mereindex=0;
    cp=(char*)kmalloc((len+1)*sizeof(char));
    if(len==0){
        cp[0]='\0';
        return cp;
    }
    bool is_finished=false;
    bool is_in_str=false;
    while (!is_finished)
    {
        if(str[mereindex]=='\0'){
            cp[cpindex]='\0';
            is_finished=true;
        }
        if(mereindex==0 && (str[mereindex]=='"' || str[mereindex]=='\'')){
            is_in_str=true;
            mereindex++;
            continue;
        }
       
        if(mereindex==len-1 && (str[mereindex]=='"' || str[mereindex]=='\'')){
            mereindex++;
            continue;
        }
        if(str[mereindex]=='\\'&&is_in_str){
            if(str[mereindex+1]=='n'){
                cp[cpindex]='\n';
                cpindex++;
                mereindex+=2;
                continue;
            }
            if(str[mereindex+1]=='r'){
                cp[cpindex]=' ';
                cp[cpindex+1]=' ';
                cpindex+=2;
                mereindex+=2;
                continue;
            }
        }
        cp[cpindex]=str[mereindex];
        cpindex++;
        mereindex++;
        
    }
    return cp;
}

void echolor(char *str,uint8_t foreground,uint8_t background){
    uint8_t iforeground;
    uint8_t ibackground;
    if(foreground==15){
        iforeground=get_term_window()->fenetre->color_foreground;
    }else{
        iforeground=foreground;
    }
    if(background==0){
        ibackground=get_term_window()->fenetre->color_intern;
    }else{
        ibackground=background;
    }
    *get_line_printed()+=WText_printstring_color(get_term_window(),prepstring(str),iforeground,ibackground);
}

void def_variable(char* str,int indexspace){
    char* id = kmalloc((indexspace+1)*sizeof(char));
    char* value = kmalloc((strlen(str)-indexspace+1)*sizeof(char));
    slice(str,id,0,indexspace);
    slice(str,value,indexspace+1,strlen(str)-indexspace-1);
    Set_user_defined_variable(id,value);
}

void execute_term_command(command_parsed_t* command){
    //WText_printstring_color(get_term_window(),command->commande,4,0);
    int indefofspace=indexOf(command->commande,'=');
    if(indefofspace>0){
       def_variable(command->commande,indefofspace);
    }
    if(strcmp(command->commande,"echo")==0){
        if(command->argslen!=1){
            WText_printstring_color(get_term_window(),"\necho needs 1 arguments have: ",4,0);
            char buf[3];
            uint8_to_string(command->argslen,buf);
            WText_printstring(get_term_window(),buf);
            return;
        }
        //WText_printstring(get_term_window(),"\n");

        *get_line_printed()+=WText_printstring(get_term_window(),prepstring(command->args[0]));
    }else if(strcmp(command->commande,"echolor")==0){
        if(command->argslen!=3){
            WText_printstring_color(get_term_window(),"\necholor needs 3 arguments have: ",4,0);
            char buf[3];
            uint8_to_string(command->argslen,buf);
            WText_printstring(get_term_window(),buf);
            return;
        }
        echolor(command->args[0],string_to_uint8(command->args[1]),string_to_uint8(command->args[2]));
    }else if(strcmp(command->commande,"clear")==0){
        WText_clear(get_term_window());
        enable_cursor();
    }else if(strcmp(command->commande,"snake")==0){
        Snake_init();
    }else if(strcmp(command->commande,"game2048")==0){
        init_2048();
    }else if(strcmp(command->commande,"palette")==0){
        for(uint8_t i=0;i<254;i++){
            
            M13h_draw_rectangle((i*20)%VGA_WIDTH,((i*20)/VGA_WIDTH)*20,20,20,i);
        }  
    }
}

