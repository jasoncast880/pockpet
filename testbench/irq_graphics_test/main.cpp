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

void color_screen(uint8_t flag){
    uint8_t colByte1=0x07; //red
    uint8_t colByte2=0xe0;
    if(gpio_flag==0x02){
        colByte1=0xf8;
        colByte2=0x00;
    }
    ili9341_writeCommand(RAM_WR);
    for(int i=0;i<(320*240);i++){
        ili9341_writeData(colByte1);
        ili9341_writeData(colByte2);
    }
    ili9341_writeCommand(NOOP);
}

int main() {
    stdio_init_all();

    //spi0 bus initializers, disabled temporarily
    ili9341_initialize(17,20,21,19,6,16);

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

    ili9341_writeCommand(RAM_WR);
    for(int i=0;i<(320*240);i++){
       ili9341_writeData(0xf8);
        ili9341_writeData(0x00);
    }
    ili9341_writeCommand(NOOP);

    while(true){
        color_screen(gpio_flag);
        sleep_ms(500);
    }

    return 0;
}
