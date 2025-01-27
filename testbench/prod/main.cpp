//prod testbench
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


//global var declaration
Tileset amp_tileset, rook_tileset;
Base base;
Font rook_font;

void tileset_font_initializer(){
    //later make this global, for now all display functionalities are in this function
    amp_tileset = Tileset(16, (uint8_t*)&ampalaya_tileset_16[0]);
    rook_tileset = Tileset(16, (uint8_t*)&rook_tileset_16[0]);

    static char charArr[] = {
         '/','.','-',',','+','*',')','(','\'','%','$','#','\"','!',' ',
        '?','>','=','<',';',':','9','8','7','6','5','4','3','2','1','0',
        'O','N','M','L','K','J','I','H','G','F','E','D','C','B','A','@',
        '_','^',']','\\','[','Z','Y','X','W','V','U','T','S','R','Q','P',
        'o','n','m','l','k','j','i','h','g','f','e','d','c','b','a','~', //last is placeholder
        '~','~','}','|','{','z','y','x','w','v','u','t','s','r','q','p' //first two in this row are placeholders
    };

    base = Base(&amp_tileset, (uint8_t*)&tile_bg_16[0]);
    rook_font = Font(&rook_tileset, &charArr[0], (sizeof(charArr)/sizeof(char)));
}

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

    /* */
    sleep_ms(2000);
    printf("GO\n");

    tileset_font_initializer();
    base.render();
    rook_font.printFont(10,10,"peepee");

    return 0;
}
