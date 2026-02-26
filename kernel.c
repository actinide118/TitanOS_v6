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
        print_string(buffer);
    }
}

void start_kernel (){
    isr_install();
    irq_install();
    char ch[] = "e";
    set_char_at_video_memory(*ch,(2*80+79)*2);
    set_cursor((1*80+3)*2);
    print_string_color("hello, TitanOS started",0x9F);
    set_cursor((2*80+2)*2);
    cadre_de_couleur(0x20);
    print_string("(\\  /)\n \\\\//\n /..\\\n \\__/ ");
    print_string("\n> ");
}
void execute_command(char* string[]){
    historique[pointeur_d_historique]=*string;
    pointeur_d_historique++;
    if(strcmp(string[0], "end")== 0){
        print_string("BYE");
        port_byte_out(0x3C0, 0x00);
        asm volatile("hlt");
    }else if(strcmp(string[0], "clear")==0){
        clear_screen();
        cadre_de_couleur(0x20);
        set_cursor((1*80+3)*2);
        print_string_color("hello, TitanOS started",0x9F);
        set_cursor((2*80+3)*2);
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
    }else if(strcmp(string[0],"page")==0){/* Lesson 22: Code to test kmalloc, the rest is unchanged */
        u32 phys_addr;
        u32 page = kmalloc(1000, 1, &phys_addr);
        char page_str[16];
        int_to_ascii(page, page_str);
        char phys_str[16];
        int_to_ascii(phys_addr, phys_str);
        print_string("Page: ");
        print_string(page_str);
        print_string(", physical address: ");
        print_string(phys_str);
        print_string("\n");
    }else if(strcmp(string[0],"int")==0){
        uint8_t nb_int = string_to_uint8(string[1]);
        trigger_interrupt(nb_int);
    }else if(strcmp(string[0],"logo")==0){
        print_string("(\\  /)\n \\\\//\n /..\\\n \\__/ \n");
        print_string(" __________    __  __________    ____       ____      __  _______   _______\n|___   ____|  /_/ |____   ___|  / _  |     /    |    / / |   __  | |  _____|\n   /  /      __       /  /     / /_\\ |    /  /| |   / /  |  |  | | |  |_____\n  /  /      /  /     /  /     / ____ |   /  / | |  / /   |  |  | | |______  |\n /  /      /  /     /  /     / /   | |  /  /  | | / /    |  |__| |    ____| |\n/  /      /  /     /  /     / /    | | /  /   | |/ /     |_______| |________|\n");
    }else if(strcmp(string[0],"game")==0){
        init_game();
    }else if(strcmp(string[0],"random")==0){
        if(seed == 0){
            seed = get_tick();
        }
        random(&seed);
        char buf[10];
        uint32_to_string(seed, buf);
        print_string(buf);
    }else if(strcmp(string[0],"2Dtest")==0){
        clear_screen();
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
    }else{
        print_string("bad command");
    }
}