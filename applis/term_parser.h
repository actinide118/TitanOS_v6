#include <stdint.h>

typedef struct cmd{
    char* commande;
    uint8_t id;
    uint8_t argslen;
    char** args;

} command_parsed_t;

typedef struct returnstruct{
    command_parsed_t** arr;
    uint8_t len;
}returnstruct_t;

returnstruct_t* parse_command_line(char* command_line);
void free_command_parsed(command_parsed_t* command_parsed );
void free_return_struct(returnstruct_t* parsed_cmd);