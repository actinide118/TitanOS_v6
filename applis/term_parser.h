#include <stdint.h>

typedef struct cmd{
    char* commande;
    uint8_t id;
    uint8_t dependencie;
    uint8_t dependonhim;
    uint8_t argslen;
    char** args;

} command_parsed_t;

typedef struct returnstruct{
    command_parsed_t** arr;
    uint8_t len;
}returnstruct_t;

returnstruct_t* parse_command_line(char* command_line);