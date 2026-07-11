/*
 * =====================================================================================
 *
 *       Filename:  terminal_executor.c
 *
 *    Description:  File containing code to execute command by command a terminal line parsed in the term_parser.c
 *
 *        Version:  1.0
 *        Created:  16/06/2026 20:24:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "terminal_executor.h"
#include "../util/util.h"
#include "../util/stringmanipulation.h"
#include "../snake/main.h"
#include "../2048/main2048.h"
#include "../cpu/cpuid.h"
#include "../cpu/features.h"
#include "../driver/memory.h"
#include "../driver/vga.h"
#include "../writing/supplier.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  prepstring
 *  Description:  Function that check if the string start by $ and get the value of the variable or escape characters like \n and \r
 * =====================================================================================
 */
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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  echolor
 *  Description:  Print a string of color if the foreground value is set to 15 it's replaced by the current terminal foreground color and if the background color is set to 0 it's replaced by the current terminal background color
 * =====================================================================================
 */
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  def_variable
 *  Description:  Split the line and isolate id and value components in order to add them to the terminal variable list
 * =====================================================================================
 */
void def_variable(char* str,int indexspace){
    char* id = kmalloc((indexspace+1)*sizeof(char));
    char* value = kmalloc((strlen(str)-indexspace+1)*sizeof(char));
    slice(str,id,0,indexspace);
    slice(str,value,indexspace+1,strlen(str)-indexspace-1);
    Set_user_defined_variable(id,value);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  execute_term_command
 *  Description:  Receive a command_parsed structure and execute it 
 * =====================================================================================
 */
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
    }else if(strcmp(command->commande,"cpuidtest")==0){
        cpuid_return_t rt;
        cpuid_execute(0,0,&rt);
        char* buf=kmalloc(20);
        uint32_to_string(rt.eax,buf);
        WText_printstring(get_term_window(),"CPUID max leaf:");
        WText_printstring(get_term_window(),buf); 
        WText_printstring(get_term_window(),"\n"); 
        buf=kmalloc(13);
        get_vendor_name(buf);
        WText_printstring(get_term_window(),"Vendor: ");
        WText_printstring(get_term_window(),buf); 
        WText_printstring(get_term_window(),"\n"); 
        free(buf);
        /* The following would give good result only if the processor is older than a Pentium IV and "modern" enough to support that feature
        
        uint32_t high;
        uint32_t low;
        get_serial_number(&low,&high);
        buf=kmalloc(10);
        WText_printstring(get_term_window(),"Serial number: ");
        uint32_to_hex(high,buf);
        WText_printstring(get_term_window(),buf);
        uint32_to_hex(low,buf);
        WText_printstring(get_term_window(),buf);
        WText_printstring(get_term_window(),"\n");
        free(buf);*/
        cpuid_execute(1,0,&rt);
        uint32_t edx;
        uint32_t ecx;
        buf=kmalloc(10);
        WText_printstring(get_term_window(),"Feature flags ");
        uint32_to_hex(edx,buf);
        WText_printstring(get_term_window(),buf);
        uint32_to_hex(ecx,buf);
        WText_printstring(get_term_window(),buf);
        WText_printstring(get_term_window(),"\n");
        free(buf);
        buf=kmalloc(10);
        uint32_to_string(get_intel_field_value(INTEL_NUMBER_CLFLUSH_LINE_SIZE),buf);
        WText_printstring(get_term_window(),"clflush line size: ");
        WText_printstring(get_term_window(),buf);
        WText_printstring(get_term_window(),"\n");
        /*if ( check_feature(INTEL_PAE,VENDOR_INTEL)==true ) {
                WText_printstring(get_term_window(),get_intel_features_name (INTEL_PAE));
        }*/
    }else if(strcmp(command->commande,"testbitmap")==0){
        uint8_t bitmap[7]={
          0b00011000,
          0b00100100,
          0b01011010,
          0b10100101,
          0b01011010,
          0b00100100,
          0b00011000,
        };
        M13h_put_binary_in_uint8_bitmap(0,0,8,7,0,4,bitmap);
    }else if(strcmp(command->commande,"checkcpu")==0){
      if(command->argslen!=1){
        WText_printstring(get_term_window(),"need 1 argument \n");
        return;
      } 
      uint16_t id=get_intel_index(command->args[0]);
      if(id==((2<<16)-1)){
        WText_printstring(get_term_window(),"Invalid name\n");
        return;
      }
      if(check_feature(id,VENDOR_INTEL)){
        WText_printstring(get_term_window(),"Feature present\n");

      }else{
        WText_printstring(get_term_window(),"Feature absent\n");

      }

    }else if(strcmp(command->commande,"flags")==0){
        for(uint16_t i=0;i<64;i++){
          if(check_feature(i,VENDOR_INTEL)){
            WText_printstring(get_term_window(),get_intel_features_name(i));
            WText_printstring(get_term_window()," ");
          }
          

        }
        WText_printstring(get_term_window(),"\n");
    }
}

