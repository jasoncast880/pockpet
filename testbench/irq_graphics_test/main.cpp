//irq graphics testbench
#include <stdio.h>
#include "pico/stdlib.h"

#include "rook_tileset_16.h"
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

#include "ili9341.h"
#include "FrameHandler.h"

#define DEBOUNCE_DELAY_MS 50
//global vars
static uint8_t gpio_flag = 0x00;
static uint32_t time_0 = 0;

void melon_animate_sm();
void fill_screen();


void gpio_callback(uint gpio, uint32_t events){
    uint32_t time_1 = to_ms_since_boot(get_absolute_time());

    if((gpio==0)&&((time_1-time_0)>=DEBOUNCE_DELAY_MS)){
        printf("gpio0 pressed\n");
        gpio_flag = 0x00;

        //timing reg adjustment: this resource might be vulnerable to race conditions in the future
        time_0=time_1;
    }
    else if((gpio==2)&&((time_1-time_0)>=DEBOUNCE_DELAY_MS)){
        printf("gpio2 pressed\n");
        gpio_flag = 0x02;
        
        time_0=time_1;
    }
}

void melon_animate_sm(){ //animation state mach
    Tileset* melon_set_ptr=new Tileset(16, (uint8_t*)ampalaya_tileset_16);
    Tilemap amp_tilemap1(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_1_16);
    Tilemap amp_tilemap2(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_2_16);
    Tilemap amp_tilemap3(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_3_16);
    Tilemap amp_tilemap4(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_4_16);

    Tilemap amp_tilemap5(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_5_16);
    Tilemap amp_tilemap6(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_6_16);
    Tilemap amp_tilemap7(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_7_16);
    Tilemap amp_tilemap8(15*16, 1*16, 4, 4, melon_set_ptr, (uint8_t*) melon_spritemap_8_16);

    Tilemap map_arr[] = { amp_tilemap1, amp_tilemap5,amp_tilemap2,amp_tilemap6,     amp_tilemap3, amp_tilemap7, amp_tilemap4, amp_tilemap8};

    int i=0;
    while(1){
        if(i>7){
            i=0;
        }
        map_arr[i].render();
        sleep_ms(250);
        i++;
    }
}

void fill_screen(){ //fill the screen with the base-tilemap
    Tileset* melon_set_ptr=new Tileset(16, (uint8_t*)ampalaya_tileset_16);
    Tilemap tilemap(melon_set_ptr, (uint8_t*)tile_bg_16);

    tilemap.render();
}

//how do i explicitly delete these obj's w out bloat???? A: i think that they are deleted automatically once they go out of scope...

int main() {
    stdio_init_all();

    //spi0 bus initializers, disabled temporarily
    ili9341_initialize(17,20,21,19,18,16);

    //pins gp0, gp2
    gpio_init(0);
    gpio_set_dir(0, GPIO_IN);
    gpio_pull_up(0);
    gpio_set_irq_enabled_with_callback(0,GPIO_IRQ_EDGE_FALL,true,gpio_callback);

    gpio_init(2);
    gpio_set_dir(2, GPIO_IN);
    gpio_pull_up(2);
    gpio_set_irq_enabled_with_callback(2,GPIO_IRQ_EDGE_FALL,true,gpio_callback);

    sleep_ms(2000);
    printf("GO\n");

    static uint8_t temp_gpio_flag = 0x00; //comparison flag for utility

    fill_screen();
    sleep_ms(100);
    melon_animate_sm();

    //base initialization
    //sprite initialization;sprite must alter Base:mapGuide to mask it's tiles relative to coord
    //sprite render pass over the vram
    //{MOTION? (yes)} : rework the fields in sprite to reflect accurate positional, tileset data
    //sprite changes base:mapGuide to mask its tiles relative to NEW coords
    //do a base render pass to compare old tileguide to new tile guide: which tiles to cover with the original 'base tilemap'

    return 0;
}
