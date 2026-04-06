#include "terminal_executor.h"

void execute_term_command(command_parsed_t* command){
    if(strcmp(command->commande,"echo")==0){
        WText_printstring(get_term_window(),"\n");
        WText_printstring(get_term_window(),command->args[0]);
    }
}
