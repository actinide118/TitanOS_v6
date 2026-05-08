#include "case.h"

void fonction_line_by_line(game_case_t* new_case,uint8_t direction,uint8_t step,uint8_t x,uint8_t y);
void fonction_transition(game_case_t* new_case,uint8_t direction,uint8_t step,uint8_t x,uint8_t y);
void rotate_bitmap(game_case_t* mere,game_case_t* fille);
game_case_t* rotate_bitmap_nb(game_case_t* mere,uint8_t nb);
game_case_t* duplicate_bitmap(game_case_t* mere);
uint8_t get_needed_rotation(uint8_t from,uint8_t to);