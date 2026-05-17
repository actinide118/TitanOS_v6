#include "main2048.h"
#include "init.h"
#include "../event/evt_queu.h"
#include "../kernel.h"
#include "../util/alea.h"
#include "../driver/memory.h"
#include "2048sprites.h"
#include "../driver/vga.h"

uint8_t* board;
uint8_t* select_from;

void init_2048(void){
    init_2048_board();
    board=kmalloc(16);
    select_from=kmalloc(16);
    for ( uint8_t i=0;i<16 ;i++ ) {
      board[i]=255;
      select_from[i]=255;
    }
    spawn_case();
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_case
 *  Description: print a 2048 case at a given place (0-15) 
 * =====================================================================================
 */
        void
put_case (uint8_t place,uint8_t value )
{
     /*M13h_put_color_bitmap(65,5,get_sprite2048(0)->width,get_sprite2048(0)->height,get_sprite2048(0)->color);*/
//             M13h_draw_rectangle(65+j*45,5+i*50,35,40,15);
        
  M13h_put_color_bitmap(65+(place%4)*45,5+(place/4)*50,get_sprite2048(value)->width,get_sprite2048(value)->height,get_sprite2048(value)->color);
}		/* -----  end of function put_case  ----- */

void spawn_case(){
  uint32_t rnd = random(get_seed());
  uint32_t rnd2= random(rnd);
  set_seed(rnd2);
  for (uint8_t i =0 ;i<16 ;i++ ) {
    select_from[i]=255;
  }
  uint8_t index=0;
  for (uint8_t i =0 ;i<16 ;i++ ) {
    
    if (board[i]==255 ) {
      select_from[index]=i;
      index++;
    }
  }
  
  if ( index==0 ) {
          return;
  }
  board[select_from[rnd%index]]=rnd2%2;
  put_case(select_from[rnd%index],rnd2%2);
}



