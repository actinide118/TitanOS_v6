#include "2048sprites.h"
#include "../writing/supplier.h"
#include "../driver/memory.h"

game_case_t* sprites2048;

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

uint8_t* binary_bitmap_to_color_bitmap(bool* bitmap, uint8_t color,uint8_t color2, uint8_t width, uint8_t height){
    uint8_t* new_sprite = kmalloc(width*height);
    for (uint8_t i = 0; i < height; i++) {
        for (uint8_t j = 0; j < width; j++) {
            new_sprite[i*width+j] = bitmap[i*width+j] ? color : color2;
        }
    }
    return new_sprite;
}

void init_sprite(){
    sprites2048 = kmalloc(3 * sizeof(game_case_t));
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
    //free(not_upscaled_sprite);
}

game_case_t* get_sprite2048( uint8_t nb){
    return &sprites2048[nb];
}