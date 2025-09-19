#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#ifdef __cplusplus
extern "C"{
#endif

//gp defines; 
//ili9341 modes, reg addressing refer to strionix ili9341 10.1 table

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

/* Mem Access CTL Register: MY-MX-MV-ML-BGR-MH-X-X
 * MY - Row Addr Order
 * MX - Column Addr Order
 * MV - Row/Column Order
 * ML - Vertical Refresh Order
 * BGR - RGB/BGR Order: 
 * MH - Horizontal Refresh Order
 * X - DC, assume 0!!!!
 * for ampalaya purposes
 * screen is rotated 90deg CCW && RGB-big endian
 * thus set:
 * MADCTL = 0 1 1 0 1 1 / 0 0 == 0x68
 */
#define MADCTL  0x36 /* see above.. */

//scrolling-related registers here
#define VSCR_DEF 0x33 /* config vert. scrolling */
#define VSCR_ADD 0x37 /* assign vert. scrolling pointer */

#define CASET   0x2A /* Column Address SET */
#define RASET   0x2B /* Row Address SET */

#define RAM_WR  0x2C /* Write to VRAM */

#define FRMCTR1 0xB1 /* frame rate control */
#define PIXSET  0x3A /* config color format */

//define subroutine initialization sequnces here;
//these sequences should be preset parameters for commands; not commands themselves
#define PWR_INIT_SEQ_1 {1,2,3,4} //example
#define PWR_INIT_SEQ_2 {1,2,3,4} //example

#define VRAM_INIT_SEQ_1 {1,2,3,4} //example
#define VRAM_INIT_SEQ_2 {1,2,3,4} //example
                                  
#define GAMMA_CORR_INIT_SEQ_1 {1,2,3,4} //example

//internal funcs
static void ili9341_hard_reset();
static void ili9341_init_sub_pwr(); 
static void ili9341_init_sub_vram(); 

static void startup_video_sequence(); //

void ili9341_initialize(int8_t cs,int8_t rst,int8_t dc); //running on spi0 bus
void ili9341_writeCommand(uint8_t commandByte);
void ili9341_writeData(uint8_t dataByte);
void ili9341_writeDataBuffer16(uint16_t* dataBuf, size_t len);

void ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h); 
void ili9341_setScrollWindow(uint16_t tfa, uint16_t vsa, uint16_t bfa); 
void ili9341_setScrollPtr(uint16_t vsp); //page 123 of strionix manual
void ili9341_exitScrollMode(); 

//for locking access to the spi0 bus
void ili9341_setCS_HIGH();
void ili9341_setCS_LOW();

#ifdef __cplusplus
}
#endif                                                       
