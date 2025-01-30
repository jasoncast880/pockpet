//gp-graphics testbench
#include <stdio.h>
#include "pico/stdlib.h"

#include "rook_tileset_16.h"
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

#include "ili9341.h"
#include "FrameHandler.h"

//serves as a testbench for the things
void graphics_test(){ //testing the HAL abstractions
    ili9341_setAddrWindow(40,40,150,150);
    ili9341_writeCommand(RAM_WR);
    uint8_t red_kek[2] = {0xF8,0x00};
    for(int i = 0; i < (150*150);i++){
        ili9341_writeData(red_kek[0]);
        ili9341_writeData(red_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(50,50,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t grn_kek[2] = {0x07,0xE0};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(grn_kek[0]);
        ili9341_writeData(grn_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(160,50,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t ylo_kek[2] = {0xFF,0xC0};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(ylo_kek[0]);
        ili9341_writeData(ylo_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    ili9341_setAddrWindow(160,100,20,20);
    ili9341_writeCommand(RAM_WR);
    uint8_t blu_kek[2] = {0x00,0x1F};
    for(int i = 0; i < (20*20);i++){
        ili9341_writeData(blu_kek[0]);
        ili9341_writeData(blu_kek[1]);
        sleep_ms(1);
    }
    ili9341_writeCommand(NOOP);

    sleep_ms(2000);
}

void tileset_demo(){ 
    Tileset* amp_tileset = new Tileset(16,(uint8_t*)ampalaya_tileset_16);

    amp_tileset->renderByIndex(10,10,3);
    
}

void tilemap_demo(){
    Tileset* amp_tileset_ptr = new Tileset(16, (uint8_t*)ampalaya_tileset_16);
    Tilemap amp_tilemap(amp_tileset_ptr, (uint8_t*)tile_bg_16);
    amp_tilemap.render();
}

int main() {
    stdio_init_all();

    sleep_ms(4500);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,18,16);

    tileset_demo();

}
