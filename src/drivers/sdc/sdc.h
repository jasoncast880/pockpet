#ifndef SDC_H
#define SDC_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

//gp defines; 

//general-purpose registers
#define NOOP    0x00
#define SWRESET 0x01

//power&display modes
#define SLPIN   0x10 /* Sleep ON */
#define SLPOUT  0x11 /* Sleep OFF */
#define PTLON   0x12 /* Partial mode ON */
#define NORON   0x13 /* Normal mode ON */

#define INVOFF  0x20 /* Display Inversion ON */
#define INVON   0x21 /* Display Inversion OFF */
#define DISPOFF 0x28 /* Display OFF */
#define DISPON  0x29 /* Display ON */

//internal funcs
static void sd_hard_reset();
static void sd_init_sub_pwr(); 

void sd_initialize(int8_t cs,int8_t rst,int8_t dc); //running on spi0 bus
void sd_writeCommand(uint8_t commandByte);
void sd_writeData(uint8_t dataByte);
void sd_writeDataBuffer8(uint8_t* dataBuf, size_t len);
void sd_writeDataBuffer16(uint16_t* dataBuf, size_t len);

//for locking access to the spi0 bus
void sd_setCS_HIGH();
void sd_setCS_LOW();

#endif //SDC_H
