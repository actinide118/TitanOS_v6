#include "driver/vga.h"
#include <stdint.h>
#include "cpu/isr.h"
#include "cpu/idt.h"
#include "driver/keyboard2.h"
#include "cpu/timer.h"
#include "driver/ata.h"
#include "./util/util.h"
#include "./driver/ports.h"
#include <stddef.h>
#include "applis/move.h"
#include "util/alea.h"
#include "2DGRTOS/main.h"
#include "./writing/supplier.h"
#include "./util/stringmanipulation.h"
#include "./gui/window.h"
#include "./driver/memory.h"
#include "./gui/text_window.h"
#include "./applis/terminal.h"

uint32_t seed = 0;
char *historique[200];
int pointeur_d_historique = 0;

void trigger_interrupt(uint8_t interrupt_number) {
    __asm__ (
        "movb %0, %%al\n\t"  // Déplacer le numéro d'interruption dans le registre AL
        "int $0x1"          // Appel de l'interruption (par exemple, appel système Linux)
        :
        : "r"(interrupt_number)  // Passe l'opérande en tant que registre ou mémoire
        : "al", "memory"         // Indique que le registre AL et la mémoire peuvent être modifiés
    );
}

void display_sector(uint16_t* sector_data, size_t length) {
    char buffer[6];  // Buffer pour contenir la version en chaîne de chaque nombre (max 7 caractères + '\0')
    for (size_t i = 0; i < length; i++) {
        uint16_to_hex(sector_data[i], buffer);
        M3h_print_string(buffer);
    }
}

uint32_t get_seed(){
    if(seed == 0){
            seed = get_tick();
    }
    return seed;
}
void set_seed(uint32_t value){
    seed=value;
}

void start_kernel (){
    isr_install();
    irq_install();
    init_struct();
    char ch[] = "e";
    M3h_set_char_at_video_memory(*ch,(2*80+79)*2);
    M3h_set_cursor((1*80+3)*2);
    M3h_print_string_color("hello, TitanOS started",0x9F);
    M3h_set_cursor((2*80+2)*2);
    M3h_cadre_de_couleur(0x20);
    M3h_print_string("(\\  /)\n \\\\//\n /..\\\n \\__/ ");
    M3h_print_string("\n> ");
}
bool execute_command(char* string[]){
    historique[pointeur_d_historique]=*string;
    pointeur_d_historique++;
    if(strcmp(string[0], "end")== 0){
        print_string("BYE");
        port_byte_out(0x3C0, 0x00);
        asm volatile("hlt");
    }else if(strcmp(string[0], "clear")==0){
        if(Is_13h()){
            M13h_clear_screen(get_background());
            return false;
        }
        M3h_clear_screen();
        M3h_cadre_de_couleur(0x20);
        M3h_set_cursor((1*80+3)*2);
        M3h_print_string_color("hello, TitanOS started",0x9F);
        M3h_set_cursor((2*80+3)*2);
    }else if(strcmp(string[0], "version")==0){
        print_string("TitanOS v6");
    }else if(strcmp(string[0], "read")==0){
        uint16_t buffer [256];
        uint32_t sector = string_to_uint32(string[1]);
        read_sector(sector, buffer);
        display_sector(buffer, 256);
    }else if(strcmp(string[0], "tick")==0){
        char ascii_tick[12];
        uint32_t nb = get_tick();
        uint32_to_string(nb, ascii_tick);
        print_string(ascii_tick);
    }else if(strcmp(string[0],"cat")==0){
        uint16_t buffer [256];
        uint32_t sector = string_to_uint32(string[1]);
        read_sector(sector, buffer);
        char resultat [256];
        uint16_array_to_string(buffer, 256,resultat);
        print_string(resultat);
    }else if(strcmp(string[0],"page")==0){
       
        char* str = (char*)kmalloc(9*sizeof(char));
        str[0]='e';
        str[1]='f';
        str[2]='g';
        str[8]='2';
        char* str2 = (char*)kmalloc(9*sizeof(char));
        if((uint32_t)str2==0){
            print_string("failed memory allocation");
        }
        str2[0]='a';
        str2[8]='b';
        free(str);
        free(str2);
        return false;
    }else if(strcmp(string[0],"int")==0){
        uint8_t nb_int = string_to_uint8(string[1]);
        trigger_interrupt(nb_int);
    }else if(strcmp(string[0],"logo")==0){
        if(Is_13h()){
            print_string("Isn t supported yet");
            return false;
        }
        M3h_print_string("(\\  /)\n \\\\//\n /..\\\n \\__/ \n");
        M3h_print_string(" __________    __  __________    ____       ____      __  _______   _______\n|___   ____|  /_/ |____   ___|  / _  |     /    |    / / |   __  | |  _____|\n   /  /      __       /  /     / /_\\ |    /  /| |   / /  |  |  | | |  |_____\n  /  /      /  /     /  /     / ____ |   /  / | |  / /   |  |  | | |______  |\n /  /      /  /     /  /     / /   | |  /  /  | | / /    |  |__| |    ____| |\n/  /      /  /     /  /     / /    | | /  /   | |/ /     |_______| |________|\n");
    }else if(strcmp(string[0],"game")==0){
        if(Is_13h()){
            print_string("Doesn t work in video mode");
            return false;
        }
        init_game();
    }else if(strcmp(string[0],"random")==0){
        if(seed == 0){
            seed = get_tick();
        }
        random(seed);
        char buf[10];
        uint32_to_string(seed, buf);
        print_string(buf);
    }else if(strcmp(string[0],"2Dtest")==0){
        if(Is_13h()){
            print_string("Doesn t work in video mode");
            return false;
        }
        M3h_clear_screen();
        renderer();
    }else if(strcmp(string[0],"hist")==0){
        for (int i = 0; i<200;i++){
            if( historique[i] != "S "){
            print_string(historique[i]);}
        }
    }else if(strcmp(string[0],"fat_format")==0){
        uint16_t buffer[256] = {0xAA, 0xBB,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,0x34,}; // Données "AA BB" en hexadécimal

        // Formater en FAT32
        fat32_format();

        // Écrire un fichier
        fat32_write_file(0, buffer);
    }else if(strcmp(string[0],"video")==0){
        if(Is_13h()){
            print_string("already in video mode");
            return false;
        }
        switch_to_13h();
        M13h_clear_screen(0);
        M13h_print_string("Hello \n world");
    }else if(strcmp(string[0],"touppercase")==0){
        to_upper_case(string[1]);
        print_string(string[1]);
    }else if(strcmp(string[0],"tolowercase")==0){
        to_lower_case(string[1]);
        print_string(string[1]);
    }else if(strcmp(string[0],"fenetre")==0){
        if(!Is_13h()){
            print_string("only work in graphic mode");
            return false;}
        struct window mywindow={
            .usable_height=180,
            .usable_width=300,
            .color_border=0x3,
            .color_title=0x7,
            .color_intern=15,
            .color_foreground=0x7,
            .title="hj",
        };
        if(!create_window(&mywindow)){
            print_string("failed");
        }else{
            return true;
        }
        
    }else if(strcmp(string[0],"fenetretext")==0){
        if(!Is_13h()){
            print_string("only work in graphic mode");
            return false;}
        text_window_t* fen=WText_create_window(100,200,"programme",3,15,15,5);
        W_scroll(fen->fenetre,8);
        WText_printstring(fen,"cc");
        WText_erase_last_line(fen);
        WText_erase_last_line(fen);
        WText_printstring(fen,"coucou");
        WText_printstring(fen,"cc la mifa\ndeuxieme ligne\net ensuite\net ensuite1\net ensuite2\net ensuite3\net ensuite4\net ensuite5\net ensuite6\net ensuite7\net ensuite8\net ensuite9\net ensuite10");
        WText_erase_last_line(fen);
        WText_erase_last_line(fen);
        return true;
    }else if(strcmp(string[0],"term")==0){
        if(!Is_13h()){
            print_string("only work in graphic mode");
            return false;}
        Term_init("");
    }else if(strcmp(string[0],"slice")==0){
        char first[8]="bonjour ";
        char sc[3];
        slice(first,sc,1,2);
        print_string(sc);
    }else{
        print_string("bad command");
    }
}

