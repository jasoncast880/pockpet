//gp-graphics testbench
#include <stdio.h>
#include "pico/stdlib.h"

#include "rook_tileset_16.h"
#include "ampalaya_tileset_16.h"
#include "tilemaps.h"

#include "jetSprite_16.h" //tileset for jet demo sprite

#include "ili9341.h"
#include "FrameHandler.h"

void debug_print(){
    for(int i=0; i<=100; i++){
//        printf("%d",pepe);
    }
}

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

Tileset amp_tileset, rook_tileset, jet_tileset;
Base baseSprite;
Sprite testSprite;
//Font rookFont;

void tileset_font_init(){
    amp_tileset = Tileset(16, (uint8_t*)&ampalaya_tileset_16[0], 40);
    baseSprite = Base((Tileset*)&amp_tileset, (uint8_t*)&tile_bg_16[0]);

    jet_tileset = Tileset(16, (uint8_t*)&jetSprite_16[0], 16);

    //gonna add the font later
}

void tileset_test(Tileset* tileset,int rows, int columns){
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<columns; j++){
            tileset->render(j*16,i*16,(i*columns)+j);
        }
    }
    sleep_ms(1000);
}

int main() {
    stdio_init_all();

    sleep_ms(4500);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,18,16); //cs, rst, dc, mosi, sclk, miso

    tileset_font_init();

    //tileset test OK
    sleep_ms(1000);
    tileset_test((Tileset*)&amp_tileset,5,6);
    printf("base tileset render ok\n");
    
    //base test OK
    sleep_ms(1000);
    baseSprite.render();
    printf("base render ok\n");

    testSprite = Sprite((Base*)&baseSprite, 100, 16*3, 2 , 2, (Tileset*)&jet_tileset, (uint8_t*)&demo_spritemap_1[0]);
    sleep_ms(1000);
    printf("sprite constructor ok\n");

    testSprite.render();
    sleep_ms(500);
    printf("first render OK\n");
    
    while(true){ 
        testSprite.render((uint8_t*)&demo_spritemap_1[0]);
        sleep_ms(500);
        printf("1 ");

        testSprite.render((uint8_t*)&demo_spritemap_2[0]);
        sleep_ms(500);
        printf("2 ");

        testSprite.render((uint8_t*)&demo_spritemap_3[0]);
        sleep_ms(500);
        printf("3 ");

        testSprite.render((uint8_t*)&demo_spritemap_4[0]);
        sleep_ms(500);
        printf("4 ");

        printf("render loop ok\n");
    }

    // needs to test how the destructors perform. check for memory leakage
    
    /*
    sleep_ms(2000);
    testSprite.render();
    printf("sprite tileset render ok\n");
    */
}
