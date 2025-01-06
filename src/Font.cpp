#include "Font.h"

struct Char_16{ //for 16 pix characters
    char glyph;
    uint8_t* bufPtr; //for now assume this is a thing
    size_t len=16*16*2; //16 pix by 16 pix, 2 bytes per pixel
};

Char_16 rookFont[100];

void mapCharToBuf(char c, uint8_t* buf){
    //HASH MAPPS BABY
    size_t index = static_cast<size_t>(c);
    rookFont[index].glyph=c;
    rookFont[index].bufPtr=buf;
}

uint8_t* getBufForChar(char c){
    size_t index = static_cast<size_t>(c);
    return rookFont[index].bufPtr;
}

void rookFont_print(std::string text, uint8_t x, uint8_t y){
    for(int i=0; i<text.size(); i++){
        ili9341_setAddrWindow(x+(16*i)+2,y,16,16);
        ili9341_writeCommand(RAM_WR);
        ili9341_writeDataBuffer(getBufForChar(text[i]),16*16*2);
        ili9341_writeCommand(NOOP);
        sleep_ms(100);
    }
}

void font_initialize(){
    char charArr[] = { //first index of is at 196-indexed tile of the rook tileset (starting from bot-left)
    ' ','!','\"','#','$','%','\'','(',')','*','+',',','-','.','/',
    '0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',
    '@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
    'P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_',
    '`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',//'`' is a placeholder....
    'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~'
    };

    //map the buffers to the chars
    for(int i=0;i<(sizeof(charArr)/sizeof(char));i++){
        mapCharToBuf(charArr[i],(uint8_t*)&rook_tileset_16[(16*16*196*2)-(i*2)]);
        sleep_ms(10); //let it digest
    }
    //now the fonts can be called by char, allowing for easy buffer access.

}

