/*
 * =====================================================================================
 *
 *       Filename:  main2048.c
 *
 *    Description: File containing the main logic of th 2048 game 
 *
 *        Version:  1.0
 *        Created:  20/05/2026 20:24:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "main2048.h"
#include "init.h"
#include "../event/evt_queu.h"
#include "../kernel.h"
#include "../util/alea.h"
#include "../driver/memory.h"
#include "2048sprites.h"
#include "../driver/vga.h"
#include "../applis/terminal.h"
#include "../util/util.h"

uint8_t* board;
uint8_t* select_from;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_case
 *  Description: print a 2048 case at a given place (0-15) 
 * =====================================================================================
 */
        void
put_case (uint8_t place,uint8_t value )
{

       
	if (value==255 ) {
		M13h_draw_rectangle(65+(place%4)*45,5+(place/4)*50,35,40,15);
		return;
	}
	M13h_put_color_bitmap(65+(place%4)*45,5+(place/4)*50,get_sprite2048(value & 0b01111111)->width,get_sprite2048(value & 0b01111111)->height,get_sprite2048(value & 0b01111111)->color);
}		/* -----  end of function put_case  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  returntoterminal
 *  Description:  
 * =====================================================================================
 */
	void
returntoterminal ()
{
	uint16_t score=0;
	
	for ( uint8_t i=0;i<16 ;i++ ) {
		if(board[i]!=255){
			score+=2<<(board[i]);
		}
	}
	char buf [30];
    	buf[0]='s';
	buf[1]='c';
	buf[2]='o';
	buf[3]='r';
	buf[4]='e';
	buf[5]=':';
	buf[6]=' ';
	uint16_to_string(score,buf+7);
    	enable_cursor();
   	Term_ret(0, buf);
	return ;
}		/* -----  end of function returntoterminal  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_board
 *  Description:  refresh the board using the board variable and put_case variable
 * =====================================================================================
 */
	void
put_board (void)
{
	
	for (uint8_t i=0 ;i<16 ;i++ ) {
		put_case(i,board[i]);
	}
	return;
}		/* -----  end of function put_board  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delete_already_merged_flag
 *  Description:  set to zero the last bit of all the case stored in board that show you connot merge the case
 * =====================================================================================
 */
	void
delete_already_merged_flag (void )
{
	
	for ( uint8_t i=0;i<16 ;i++ ) {
		
		if (board[i]==255 ) {
			continue;
			
		}
		board[i]=board[i]&0b01111111;
	}
	return;
}		/* -----  end of function delete_already_merged_flage  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  move_in_direction
 *  Description:  make the blocks stack in a given direction (0<=direction<4)
 * =====================================================================================
 */
	void
move_in_direction (uint8_t direction)
{
	if(direction>3){
		return;
	}
	bool hasmoved=false;	
	switch (direction ) {
		case 0:

			for (uint8_t i=1 ;i<4 ;i++ ) {
				for ( uint8_t j=0;j<4 ;j++ ) {
					
					if (board[i*4+j]==255 ) {
						////print_string("pass");
						continue;
					}
					bool found=false;
					uint8_t index=i-1;
					while (!found) {
						if ( board[index*4+j]!=255 ) {
							
							if (  board[index*4+j]==board[i*4+j] && ((board[i*4+j] & 0b10000000)==0 )&&  ((board[index*4+j] & 0b10000000)==0 )  ) {
								 board[index*4+j]=(1+board[index*4+j])|0b10000000;
								 board[i*4+j]=255;
								 hasmoved=true;
								 //print_string("merge");
							
							}else{
								//print_string("top");
								
								if (i!=index+1  ) {
								
								
									board[(index+1)*4+j]=board[i*4+j];
									board[i*4+j]=255;
									hasmoved=true;
								}
								
							}

							found=true;
							index--;
							continue;
						}
						
						if(index==0){
							//print_string("zero");
							board[j]=board[i*4+j];
							board[i*4+j]=255;
							found=true;
							break;
						}
						
						index--;
					}
				}
			}
			put_board();
			break;

		case 1:


			for (uint8_t i=0 ;i<4 ;i++ ) {
				for ( uint8_t j=2;j<5 ;j-- ) {
					
					if (board[i*4+j]==255 ) {
					//	//print_string("pass");
						continue;
					}
					bool found=false;
					uint8_t index=j+1;
					while (!found) {
						if ( board[i*4+index]!=255 ) {
							
							if (  board[i*4+index]==board[i*4+j] && ((board[i*4+j] & 0b10000000)==0 )&&  ((board[i*4+index] & 0b10000000)==0 )  ) {
								 board[i*4+index]=(1+board[i*4+index])|0b10000000;
								 board[i*4+j]=255;
								 hasmoved=true;
								 //print_string("merge");
							
							}else{
								//print_string("top");
								
								if (j!=index-1  ) {
								
								
									board[i*4+index-1]=board[i*4+j];
									board[i*4+j]=255;
									hasmoved=true;
								}else{
									//print_string("height");
								}
								
							}

							found=true;
							index++;
							continue;
						}
						
						if(index==3){
							//print_string("zero");
							board[i*4+3]=board[i*4+j];
							board[i*4+j]=255;
							found=true;
							break;
						}
						
						index++;
					}
				}
			}
			put_board();
			

			break;

		case 2:
			for (uint8_t i=3 ;i<5 ;i-- ) {
				for ( uint8_t j=0;j<4 ;j++ ) {
					
					if (board[i*4+j]==255 ) {
						////print_string("pass");
						continue;
					}
					bool found=false;
					uint8_t index=i+1;
					while (!found) {
						if ( board[index*4+j]!=255 ) {
							
							if (  board[index*4+j]==board[i*4+j] && ((board[i*4+j] & 0b10000000)==0 )&&  ((board[index*4+j] & 0b10000000)==0 )  ) {
								 board[index*4+j]=(1+board[index*4+j])|0b10000000;
								 board[i*4+j]=255;
								 hasmoved=true;
								 //print_string("merge");
							
							}else{
								//print_string("top");
								
								if (i!=index-1  ) {
								
								
									board[(index-1)*4+j]=board[i*4+j];
									board[i*4+j]=255;
									hasmoved=true;
								}
								
							}

							found=true;
							index++;
							continue;
						}
						
						if(index==3){
							//print_string("zero");
							board[12+j]=board[i*4+j];
							board[i*4+j]=255;
							found=true;
							break;
						}
						
						index++;
					}
				}
			}
			put_board();
			break;

		case 3:


			for (uint8_t i=0 ;i<4 ;i++ ) {
				for ( uint8_t j=1;j<4 ;j++ ) {
					
					if (board[i*4+j]==255 ) {
					//	//print_string("pass");
						continue;
					}
					bool found=false;
					uint8_t index=j-1;
					while (!found) {
						if ( board[i*4+index]!=255 ) {
							
							if (  board[i*4+index]==board[i*4+j] && ((board[i*4+j] & 0b10000000)==0 )&&  ((board[i*4+index] & 0b10000000)==0 )  ) {
								 board[i*4+index]=(1+board[i*4+index])|0b10000000;
								 board[i*4+j]=255;
								 hasmoved=true;
								 //print_string("merge");
							
							}else{
								//print_string("top");
								
								if (j!=index+1  ) {
								
								
									board[i*4+index+1]=board[i*4+j];
									board[i*4+j]=255;
									hasmoved=true;
								}else{
									//print_string("height");
								}
								
							}

							found=true;
							index--;
							continue;
						}
						
						if(index==0){
							//print_string("zero");
							board[i*4]=board[i*4+j];
							board[i*4+j]=255;
							found=true;
							break;
						}
						
						index--;
					}
				}
			}
			put_board();
			
			break;
	}				/* -----  end switch  ----- */
	return;
}		/* -----  end of function move_in_direction  ----- */
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  key_actions
 *  Description:  the callback when a key is pressed
 * =====================================================================================
 */

void key_actions(struct keymap km, uint8_t arg2){
    if(km.normal=='z' || km.normal==KEY_UP){
        move_in_direction(0);
    	spawn_case();
	put_board();
        delete_already_merged_flag();
    }else if(km.normal=='d' || km.normal==KEY_RIGHT){
        move_in_direction(1);
    	spawn_case();
	put_board();
        delete_already_merged_flag();
    }else if(km.normal=='s' || km.normal==KEY_DOWN){
        move_in_direction(2);
    	spawn_case();
	put_board();
        delete_already_merged_flag();
    }else if(km.normal=='q' || km.normal==KEY_LEFT){
        move_in_direction(3);
 	spawn_case();
	put_board();
        delete_already_merged_flag();   
    }else if(km.normal=='e'){
	    returntoterminal();
    }
}/* -----  end of function key_actions  ----- */


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



void init_2048(void){
	init_2048_board();
	board=kmalloc(16);
	select_from=kmalloc(16);
	for ( uint8_t i=0;i<16 ;i++ ) {
		board[i]=255;
		select_from[i]=255;
	}
	spawn_case();
	/*board[0]=0;
	board[1]=0;
	board[2]=1;
	board[3]=1;

	put_board();
	for (uint32_t i=0 ;i<0b1111111111111111111111 ;i++ ) {
		asm("nop");	
	}
	move_in_direction(1);
	put_board();
        delete_already_merged_flag();
	for (uint32_t i=0 ;i<0b11111111111111111111111 ;i++ ) {
		asm("nop");	
	}

	move_in_direction(1);*/
	put_board();
        delete_already_merged_flag();
	set_callback_keyboard(key_actions);

	
}
