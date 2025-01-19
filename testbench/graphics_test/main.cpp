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

void tile_test_16() { //test the tile class
    Tile tile1(16, (uint8_t*)&rook_tileset_16[512*1]);
    tile1.render(25,25);
    sleep_ms(200);
    Tile tile10(16, (uint8_t*)&rook_tileset_16[512*10]);
    tile10.render(50,25);
    sleep_ms(200);
    Tile tile12(16, (uint8_t*)&rook_tileset_16[512*12]);
    tile12.render(75,25);
    sleep_ms(200);
    Tile tile6(16, (uint8_t*)&rook_tileset_16[512*6]);
    tile6.render(100,25);
    sleep_ms(200);
}

void tile_test_16_alt() { //test the tile class
    Tile tile1(16, (uint8_t*)&ampalaya_tileset_16[512*1]);
    tile1.render(25,100);
    sleep_ms(200);
    Tile tile10(16, (uint8_t*)&ampalaya_tileset_16[512*10]);
    tile10.render(25+16,100);
    sleep_ms(200);
    Tile tile21(16, (uint8_t*)&ampalaya_tileset_16[512*21]);
    tile21.render(25+16+16,100);
    sleep_ms(200);
    
}

void tileset_demo_16(){
    Tileset* tileset_ptr = new Tileset(16, (uint8_t*)rook_tileset_16);
    for(int j = 0; j<15;j++){ //should be 
        for(int i = 0; i<16;i++){
            tileset_ptr->render(16*i, 16*j, ((16*j)+i));
            //sleep_ms(del);
        }
    }
}

void tilemap_demo(){
    Tileset* amp_tileset_ptr = new Tileset(16, (uint8_t*)ampalaya_tileset_16);
    Tilemap amp_tilemap(amp_tileset_ptr, (uint8_t*)tile_bg_16);
    amp_tilemap.render();
}

void clr_screen(){ //i gotta make a util 'HAL' file...
    ili9341_setAddrWindow(0,0,320,240);
    ili9341_writeCommand(RAM_WR);
    for(int i=0; i<=(320*240);i++){
        ili9341_writeData(0xFF); //yello
        ili9341_writeData(0xC0);
    }
    ili9341_writeCommand(NOOP);
}

//fix later
void font_demo(){
    char charArr[] = { //first index of is at 196-indexed tile of the rook tileset (starting from bot-left)
    ' ','!','\"','#','$','%','\'','(',')','*','+',',','-','.','/',
    '0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',
    '@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
    'P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_',
    '~','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',//'~' is a placeholder for a checkmark tile
    'p','q','r','s','t','u','v','w','x','y','z','{','|','}'
    };

    Tileset* rookTilesetPtr = new Tileset(16, (uint8_t*)rook_tileset_16);

    printf("%d\n",sizeof(charArr)/sizeof(char));
    Font rookFont(rookTilesetPtr,charArr,(sizeof(charArr)/sizeof(char)));
    //constructor is broken!!!!!!!!!!!!!!!!!!!!!!!
    printf("font constructor ok\n");

    rookFont.printFont(10,10,"Hello World");
    printf("printFont ok\n");
}

void sprite_base_test(){

    Tileset* rookTilesetPtr = new Tileset(16, (uint8_t*)rook_tileset_16);

    Tileset* ampTilesetPtr = new Tileset(16, (uint8_t*)ampalaya_tileset_16);
    Base* baseSprite = new Base(ampTilesetPtr, (uint8_t*)tile_bg_16);

    baseSprite->render();

    Sprite* spriteThing = new Sprite(baseSprite,16,16,2,2,ampTilesetPtr,(uint8_t*)melon_spritemap_1_16);
    spriteThing->render();

    //check initializations for now, once tested then i can test my positional system
}

int main() {
    stdio_init_all();

    sleep_ms(2000);
    printf("GO\n");

    ili9341_initialize(17,20,21,19,6,16);

    sprite_base_test(); 
   
}
