/*
 * ili9341 shouuld control all of the reg hardware assignments and 
 * be an abstraction layer for all low level writes and commands..
 * the driver should have a setup that can later be integrated into cpp
 */

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
 * MADCTL = 0 1 1 0 1 0 / 0 0 == 0x68
 */
#define MADCTL  0x36 

#define CASET   0x2A /* Column Address SET */
#define RASET   0x2B /* Row Address SET */

#define RAM_WR  0x2C /* Write to VRAM */

#define FRMCTR1 0xB1 /* frame rate control */
#define PIXSET  0x3A /* config color format */

//power control
//color control
//mem access data control registers

//define subroutine initialization sequnces here;
//these sequences should be preset parameters for commands; not commands themselves
#define PWR_INIT_SEQ_1 {1,2,3,4} //example
#define PWR_INIT_SEQ_2 {1,2,3,4} //example

#define VRAM_INIT_SEQ_1 {1,2,3,4} //example
#define VRAM_INIT_SEQ_2 {1,2,3,4} //example
                                  
#define GAMMA_CORR_INIT_SEQ_1 {1,2,3,4} //example

//gpio abstraction function implementations
extern int8_t _ILI9341_CS;
extern int8_t _ILI9341_RST;
extern int8_t _ILI9341_DC;
extern int8_t _ILI9341_MOSI;
extern int8_t _ILI9341_SCLK;
extern int8_t _ILI9341_MISO;

#define _CS_INIT gpio_init(_ILI9341_CS)
#define _RST_INIT gpio_init(_ILI9341_RST)
#define _DC_INIT gpio_init(_ILI9341_DC)
#define _MOSI_INIT gpio_init(_ILI9341_MOSI)
#define _SCLK_INIT gpio_init(_ILI9341_SCLK)
#define _MISO_INIT gpio_init(_ILI9341_MISO)

#define _CS_SetDigitalOut gpio_set_dir(_ILI9341_CS, GPIO_OUT)
#define _RST_SetDigitalOut gpio_set_dir(_ILI9341_RST, GPIO_OUT)
#define _DC_SetDigitalOut gpio_set_dir(_ILI9341_DC, GPIO_OUT)
#define _MOSI_SetDigitalOut gpio_set_dir(_ILI9341_MOSI, GPIO_OUT)
#define _SCLK_SetDigitalOut gpio_set_dir(_ILI9341_SCLK, GPIO_IN)
#define _MISO_SetDigitalOut gpio_set_dir(_ILI9341_MISO, GPIO_OUT)

//MISO not used. keep in mind

#define _SCLK_SPI_FUNC gpio_set_function(_ILI9341_SCLK, GPIO_FUNC_SPI)
#define _MOSI_SPI_FUNC gpio_set_function(_ILI9341_MOSI, GPIO_FUNC_SPI)

//internal funcs
static void ili9341_hard_reset();
static void ili9341_init_sub_pwr(); 
static void ili9341_init_sub_vram(); 

void ili9341_initialize(int8_t cs,int8_t rst,int8_t dc,int8_t mosi,int8_t sclk,int8_t miso);
void ili9341_writeCommand(uint8_t commandByte);
void ili9341_writeData(uint8_t dataByte);
void ili9341_writeDataBuffer(uint8_t* dataBuf, size_t len);
void ili9341_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h); //return the buffer size

// utility functions
void ili9341_drawFrame(uint32_t * buf, size_t len); //gp frame write; should be compatible with dma
void ili9341_write_565(uint32_t * bmpData, size_t len); //draw a bitmap from ram; optimized and shii
                                                        
#ifdef __cplusplus
}
#endif                                                       
