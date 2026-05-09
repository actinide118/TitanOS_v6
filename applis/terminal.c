#include "terminal.h"
#include "terminal_executor.h"
#include "term_parser.h"


uint8_t current_command_lenght=0;
uint8_t current_command_lines=1;
bool indicator_on=true;
bool can_change_indicator=true;
bool disabled=false;
growing_obj_t* strbuf;
int line_printed=0;
growing_obj_t* user_variables;

text_window_t* graphic_container;
char *current_command_line;

growing_obj_t* get_str_buf(){
    return strbuf;
}

int* get_line_printed(){
    return &line_printed;
}

growing_obj_t* get_user_variables(){
    return user_variables;
}

text_window_t* get_term_window(){
    return graphic_container;
}

void callback(uint32_t nb){
    if(!can_change_indicator){
        return;
    }
    if(nb%4!=0){
        return;
    }
    if(indicator_on){
        WText_erase_last_char(graphic_container);
        indicator_on=false;
    }else{
        WText_printstring(graphic_container,"|");
        indicator_on=true;
    }
}

void disable_cursor(void){
    if(indicator_on){
        WText_erase_last_char(graphic_container);
        indicator_on=false;
    }
    can_change_indicator=false;
    disabled=true;
}
void enable_cursor(void){
    can_change_indicator=true;
    disabled=false;
    if(!indicator_on){
        WText_printstring(graphic_container,"|");
        indicator_on=true;
    }
}

void keypresscallback(struct keymap ch, uint8_t flag){
    //can_change_indicator=false;
    if(indicator_on){
        indicator_on=false;
        WText_erase_last_char(graphic_container);
        
    }
    if (ch.normal == ASCII_BS) {
        current_command_lenght--;
        current_command_line[current_command_lenght]='\0';
        for(uint8_t i=0;i<=current_command_lines;i++){
            WText_erase_last_line(graphic_container);
        }
        if(!disabled){ WText_printstring(graphic_container,">");
        current_command_lines=WText_printstring(graphic_container,current_command_line);
        }
    } else if (ch.normal == ASCII_CR) {
        returnstruct_t* parsed=parse_command_line(current_command_line);
        WText_printstring(graphic_container,"\n");
        line_printed=1;
        if(parsed->len==0){
           // WText_printstring(graphic_container,"0 0 ");
        }
        for(uint8_t i=0;i<parsed->len;i++){
            execute_term_command(parsed->arr[i]);
        }
        
        if(!disabled) WText_printstring(graphic_container,">");
        for(uint8_t i=0;i<=200;i++){
            current_command_line[i] = '\0';
        }
        
        free_return_struct(parsed);
        line_printed=0;
        current_command_lenght=0;
        current_command_lines=0;
    }else if(ch.special == KEYMAP_ALPHA || ch.special == 0){
		uint8_t shift = (flag & MASK_SHIFT) ? 1 : 0;
    	uint8_t alt = (flag & MASK_ALT) ? 1 : 0;
    	uint8_t ctrl = (flag & MASK_CTRL) ? 1 : 0;
    	uint8_t capslock = (flag & MASK_CAPSLOCK) ? 1 : 0;
    	uint8_t numlock= (flag & MASK_NUMLOCK) ? 1 : 0;

        char letter;

		if(shift == 1){
			letter = ch.shifted;
		}else if(alt == 1){
			letter = ch.alted;
		}else{
			letter = ch.normal;
		}
        if(current_command_lenght==255){
            return;
        }
        current_command_line[current_command_lenght]=letter;
        current_command_lenght++;
        char str[2] = {letter, '\0'};
        current_command_lines+=WText_printstring(graphic_container,str);
    }
    //can_change_indicator=true;
}

char* Get_user_defined_variable(char* key){
    return (char*)GObj_get(user_variables,key);
}

void Set_user_defined_variable(char* key,char* value){
    GObj_set(user_variables,key,value);
}

bool Term_init(char *welcome_message){
    if(!Is_13h()){
        return false;
    }
    graphic_container=WText_create_window(186,312,"kernel command line",2,15,0,15);
    current_command_line=(char*)kmalloc(256*sizeof(char));
    for(uint8_t i=0;i<=200;i++){
            current_command_line[i] = '\0';
        }
    if(!*welcome_message=='\0'){
        WText_printstring(graphic_container,welcome_message);
    }
    user_variables=GObj_create();
    WText_printstring(graphic_container,">|");
    set_callback_tick(callback);
    set_callback_keyboard(keypresscallback);
    return true;
}

void Term_ret(uint8_t code, char* message){
    WText_clear(graphic_container);
    WText_printstring(graphic_container,message);
    set_callback_tick(callback);
    set_callback_keyboard(keypresscallback); 
    WText_printstring(graphic_container,">|");
    disabled=false;
}