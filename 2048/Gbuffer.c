/*
 * =====================================================================================
 *
 *       Filename:  Gbuffer.c
 *
 *    Description: Manipulate Graphical buffer that are not video memory 
 *
 *        Version:  1.0
 *        Created:  05/15/2026 10:21:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "Gbuffer.h"
#include "../driver/vga.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  put_bitmap_to_Gbuffer
 *  Description: put a uint8_t bitmap and display it in a buffer 
 * =====================================================================================
 */
uint8_t put_bitmap_to_Gbuffer(uint8_t* buf,uint16_t buf_width,uint16_t buf_height,uint8_t* bitmap,uint16_t x, uint16_t y, uint8_t bitmap_width,uint8_t bitmap_height){
  
  if (x+bitmap_width>buf_width || y+bitmap_height>buf_height ) {
                return 1;
  }
  for ( uint8_t i=0;i<bitmap_height ;i++ ) {    
          for (uint8_t j=0  ;j<bitmap_width ;j++ ) {
            buf[(y+i)*buf_width+x+j]=bitmap[i*bitmap_width+j];
          }
  }
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clear_Gbuffer
 *  Description: Clear a Gbuffer width a color  
 * =====================================================================================
 */
void clear_Gbuffer (uint8_t* buf, uint16_t width, uint16_t height, uint8_t color )
{
  
  for (uint16_t i=0 ;i<height ;i++ ) {
    
          for (uint16_t j=0 ;j<width ;j++ ) {
            buf[i*width+j]=color;
          }
  }
  return;
}		/* -----  end of function clear_Gbuffer  ----- */
