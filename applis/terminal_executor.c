#include "terminal_executor.h"
#include "../util/util.h"

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
    }
    WText_printstring(get_term_window(),"\n");
    WText_printstring_color(get_term_window(),str,iforeground,ibackground);
}

void execute_term_command(command_parsed_t* command){
    if(strcmp(command->commande,"echo")==0){
        if(command->argslen!=1){
            WText_printstring_color(get_term_window(),"\necho needs 1 arguments",4,0);
            return;
        }
        WText_printstring(get_term_window(),"\n");
        WText_printstring(get_term_window(),command->args[0]);
    }else if(strcmp(command->commande,"echolor")==0){
        if(command->argslen!=3){
            WText_printstring_color(get_term_window(),"\necholor needs 3 arguments",4,0);
        }
        echolor(command->args[0],string_to_uint8(command->args[1]),string_to_uint8(command->args[2]));
    }
}

