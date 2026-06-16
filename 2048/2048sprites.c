/*
 * =====================================================================================
 *
 *       Filename:  2048sprites.c
 *
 *    Description:  File containig all the methods to create the block used in the 2048
 *
 *        Version:  1.0
 *        Created:  16/06/2026 21:27:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titouan (actinide118), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "2048sprites.h"
#include "../writing/supplier.h"
#include "../driver/memory.h"
#include "../driver/vga.h"
#include "../util/util.h"
#include "Gbuffer.h"
game_case_t* sprites2048;


bool sprite16[7*8]={
false, false, true, false, false, false, true,
false, true, true, false, false, true, false,
true, false, true, false, true, false, false,
false, false, true, false, true, true, false,
false, false, true, false, true, false, true,
false, false, true, false, true, false, true,
false, false, true, false, true, false, true,
false, false, true, false, false, true, false
};

bool sprite32[7*8]={
false, true, false, false, true, true, false,
true, false, true, false, false, false, true,
false, false, true, false, false, true, false,
false, true, false, false, true, false, false,
false, false, true, false, true, false, false,
true, false, true, false, true, false, false,
false, true, false, false, true, true, true,
false, false, false, false, false, false, false
};

bool sprite64[7*8]={
false, false, true, false, true, false, true,
false, true, false, false, true, false, true,
true, false, false, false, true, false, true,
true, true, false, false, true, true, true,
true, false, true, false, false, false, true,
true, false, false, true, false, false, true,
true, false, true, false, false, false, true,
false, true, false, false, false, false, true
};


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  upscale
 *  Description: upscale a color bitmap of width * heigth size by a factor 
 * =====================================================================================
 */
uint8_t* upscale(uint8_t* sprite,uint8_t width, uint8_t height,uint8_t multiplier){
    uint8_t* new_sprite = kmalloc(width*multiplier*height*multiplier);
    for (uint8_t i = 0; i < height; i++) {
        for (uint8_t j = 0; j < width; j++) {
            for (uint8_t k = 0; k < multiplier; k++) {
                for (uint8_t l = 0; l < multiplier; l++) {
                    new_sprite[(i*multiplier+k)*width*multiplier+(j*multiplier+l)] = sprite[i*width+j];
                }
            }
        }
    }
    return new_sprite;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sort
 *  Description: sort an array of uint8_t
 * =====================================================================================
 */
void sort (uint8_t* arr,uint8_t len )
{
  if(len<=1) return;
  
  for (uint8_t target_index=0 ;target_index<len ;target_index++ ) {
    uint8_t curr = arr[target_index];
    uint8_t icurr=target_index; 
    for (uint8_t i=target_index ;i<len ;i++ ) {

     if (arr[i]<curr  ) {
      curr=arr[i];
      icurr=i;
     }
    }
    uint8_t intermediaire=arr[target_index];
    arr[target_index]=curr;
    arr[icurr]=intermediaire;
  }
  
  return;
}		/* -----  end of function sort  ----- */

uint8_t errret[3]={3,3,3};


struct count {
  uint8_t value;
  uint8_t occ;
};				/* ----------  end of struct count  ---------- */

typedef struct count count_t;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  downscale
 *  Description: Downscale a color bitmap (don't work)
 * =====================================================================================
 */
uint8_t* downscale (uint8_t* sprite,uint8_t width,uint8_t height,uint8_t multiplier )
{
  if(width%multiplier!=0&&height%multiplier!=0){    
     return errret;
  }
  uint8_t* new_sprite = kmalloc((width*height)/multiplier);
  uint8_t new_width = width/multiplier;
  uint8_t new_height = height/multiplier;
  for (uint8_t i ;i<new_height ;i++ ) {
    for (uint8_t j ;j<new_width ;j++ ) {
      uint8_t* colors=kmalloc(multiplier*multiplier);
      uint8_t index=0;
      for (uint8_t k ;k<multiplier ;k++ ) {
        for (uint8_t l ;l<multiplier ;l++ ) {
          colors[index]=sprite[(i*multiplier+k)*width+j*multiplier+l];
          index++;
        }
      }
      count_t* counting=kmalloc(multiplier*multiplier*sizeof(count_t));
      for (uint8_t nb=0 ;nb<multiplier*multiplier ;nb++ ) {
        counting[nb].occ=0;
      }
      for (uint8_t nb=0 ;nb<multiplier*multiplier ;nb++ ) {
        
        for (uint8_t nb2=0 ;nb2<multiplier*multiplier ;nb2++ ) {
          if(colors[nb]==counting[nb2].value){
            counting[nb2].occ+=1;
          }
          
          if ( counting[nb2].occ==0 ) {
                  counting[nb2].occ=1;
                  counting[nb2].value=colors[nb];
          }
        }
      }
      print_string("p");
      char buf[4];
      uint8_to_string(counting[0].value,buf);
      print_string(buf);
      new_sprite[i*new_width+j]=counting[0].value;
    }
  }
  return new_sprite;
}		/* -----  end of function downscale  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  binary_bitmap_to_color_bitmap
 *  Description:  Take two colors and a bitmap and transform it in a color bitmap. Used for function compatibility
 * =====================================================================================
 */
uint8_t* binary_bitmap_to_color_bitmap(bool* bitmap, uint8_t color,uint8_t color2, uint8_t width, uint8_t height){
    uint8_t* new_sprite = kmalloc(width*height);
    for (uint8_t i = 0; i < height; i++) {
        for (uint8_t j = 0; j < width; j++) {
            new_sprite[i*width+j] = bitmap[i*width+j] ? color : color2;
        }
    }
    return new_sprite;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  init_sprite
 *  Description:  Use higher function to generate the block of the 2048 in memory (to save disk space)
 * =====================================================================================
 */
void init_sprite(){
    sprites2048 = kmalloc(11 * sizeof(game_case_t));
    uint8_t* not_upscaled_sprite = binary_bitmap_to_color_bitmap(get_character('2')->graph, 0, 63, 7, 8);
    sprites2048[0].color = upscale(not_upscaled_sprite,7,8,5);
    sprites2048[0].width = 35;
    sprites2048[0].height = 40;
    free(not_upscaled_sprite);
    not_upscaled_sprite = binary_bitmap_to_color_bitmap(get_character('4')->graph, 0, 55, 7, 8);
    sprites2048[1].color = upscale(not_upscaled_sprite,7,8,5);
    sprites2048[1].width = 35;
    sprites2048[1].height = 40;
    free(not_upscaled_sprite);
    not_upscaled_sprite = binary_bitmap_to_color_bitmap(get_character('8')->graph, 0, 39, 7, 8);
    sprites2048[2].color = upscale(not_upscaled_sprite,7,8,5);
    sprites2048[2].width = 35;
    sprites2048[2].height = 40;
    free(not_upscaled_sprite);
    not_upscaled_sprite = binary_bitmap_to_color_bitmap(sprite16,0,14,7,8);
    sprites2048[3].color=upscale(not_upscaled_sprite,7,8,5);
    sprites2048[3].width=35;
    sprites2048[3].height=40;
    free(not_upscaled_sprite);
    not_upscaled_sprite = binary_bitmap_to_color_bitmap(sprite32,0,13,7,8);
    sprites2048[4].color=upscale(not_upscaled_sprite,7,8,5);
    sprites2048[4].width=35;
    sprites2048[4].height=40;
    free(not_upscaled_sprite);
    not_upscaled_sprite= binary_bitmap_to_color_bitmap(sprite64,0,28,7,8);
    sprites2048[5].color=upscale(not_upscaled_sprite,7,8,5);
    sprites2048[5].width=35;
    sprites2048[5].height=40;
    uint8_t* sprite128=kmalloc(35*40);
    clear_Gbuffer(sprite128,35,40,12);
    put_bitmap_to_Gbuffer(sprite128,35,40,binary_bitmap_to_color_bitmap(get_character('1')->graph,0,12,7,8),3,16, 7,8);
    put_bitmap_to_Gbuffer(sprite128,35,40,binary_bitmap_to_color_bitmap(get_character('2')->graph,0,12,7,8),14,16, 7,8);
    put_bitmap_to_Gbuffer(sprite128,35,40,binary_bitmap_to_color_bitmap(get_character('8')->graph,0,12,7,8),25,16, 7,8);

    sprites2048[6].color=sprite128;
    sprites2048[6].width=35;
    sprites2048[6].height=40;
    uint8_t* sprite256=kmalloc(35*40);
    clear_Gbuffer(sprite256,35,40,11);
    put_bitmap_to_Gbuffer(sprite256,35,40,binary_bitmap_to_color_bitmap(get_character('2')->graph,0,11,7,8),3,16, 7,8);
    put_bitmap_to_Gbuffer(sprite256,35,40,binary_bitmap_to_color_bitmap(get_character('5')->graph,0,11,7,8),14,16, 7,8);
    put_bitmap_to_Gbuffer(sprite256,35,40,binary_bitmap_to_color_bitmap(get_character('6')->graph,0,11,7,8),25,16, 7,8);
    sprites2048[7].color=sprite256;
    sprites2048[7].width=35;
    sprites2048[7].height=40;

    uint8_t* sprite512=kmalloc(35*40);
    clear_Gbuffer(sprite512,35,40,10);
    put_bitmap_to_Gbuffer(sprite512,35,40,binary_bitmap_to_color_bitmap(get_character('5')->graph,0,10,7,8),3,16, 7,8);
    put_bitmap_to_Gbuffer(sprite512,35,40,binary_bitmap_to_color_bitmap(get_character('1')->graph,0,10,7,8),14,16, 7,8);
    put_bitmap_to_Gbuffer(sprite512,35,40,binary_bitmap_to_color_bitmap(get_character('2')->graph,0,10,7,8),25,16, 7,8);
    sprites2048[8].color=sprite512;
    sprites2048[8].width=35;
    sprites2048[8].height=40;


    uint8_t* sprite1024=kmalloc(35*40);
    clear_Gbuffer(sprite1024,35,40,9);
    put_bitmap_to_Gbuffer(sprite1024,35,40,binary_bitmap_to_color_bitmap(get_character('1')->graph,0,9,7,8),1,16,7,8);
    put_bitmap_to_Gbuffer(sprite1024,35,40,binary_bitmap_to_color_bitmap(get_character('0')->graph,0,9,7,8),9,16,7,8);
    put_bitmap_to_Gbuffer(sprite1024,35,40,binary_bitmap_to_color_bitmap(get_character('2')->graph,0,9,7,8),17,16,7,8);
    put_bitmap_to_Gbuffer(sprite1024,35,40,binary_bitmap_to_color_bitmap(get_character('4')->graph,0,9,7,8),23,16,7,8);
    sprites2048[9].color=sprite1024;
    sprites2048[9].width=35;
    sprites2048[9].height=40;

    uint8_t* sprite2048=kmalloc(35*40);
    clear_Gbuffer(sprite2048,35,40,8);
    put_bitmap_to_Gbuffer(sprite2048,35,40,binary_bitmap_to_color_bitmap(get_character('2')->graph,0,8,7,8),1,16,7,8);
    put_bitmap_to_Gbuffer(sprite2048,35,40,binary_bitmap_to_color_bitmap(get_character('0')->graph,0,8,7,8),8,16,7,8);
    put_bitmap_to_Gbuffer(sprite2048,35,40,binary_bitmap_to_color_bitmap(get_character('4')->graph,0,8,7,8),17,16,7,8);
    put_bitmap_to_Gbuffer(sprite2048,35,40,binary_bitmap_to_color_bitmap(get_character('8')->graph,0,8,7,8),23,16,7,8);
    sprites2048[10].color=sprite2048;
    sprites2048[10].width=35;
    sprites2048[10].height=40;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_sprite2048
 *  Description:  Get the block corresponding to the number. the number showed and the internal number are different and are related by this relation ns=2^ni+1
 * =====================================================================================
 */
game_case_t* get_sprite2048( uint8_t nb){
    return &sprites2048[nb];
}
