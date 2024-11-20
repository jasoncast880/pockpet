#include "pico/stdlib.h"
#include "hardware/spi.h"

/*
 * ili9341 shouuld control all of the reg hardware assignments and 
 * be an abstraction layer for all low level writes and commands..
 * although i will be writing app code in c
 * the driver should have a setup that can later be integrated into cpp
 */

#pragma once

//gp defines; 
//
//ili9341 modes, reg addressing refer to strionix ili9341 10.1 table

//general-purpose registers
#define NOP     0x00
#define SWRESET 0x01
#define RDDID   0x04
#define RDDST   0x09

//???
#define RDDPM   0x0A
#define RDD_MADCTL 0x0B
#define RDD_COLMOD 0x0C
#define RDDIM   0x0D
#define RDDSM   0x0E

//modes
#define SLPIN   0x10
#define SLPOUT  0x11
#define PTLON   0x12
#define NORON   0x13
#define INVOFF  0x20
#define INVON   0x21
#define GAMSET  0x26
#define DISPOFF 0x28
#define DISPON  0x29

//row, column set registers
#define CASET   0x2A
#define RASET   0x2B
//there are more...

//frame rate control
//power control
//color control
//mem access data control registers

//define subroutine initialization sequnces here
#define PWR_INIT_SEQ_1 {1,2,3,4} //example
#define PWR_INIT_SEQ_2 {1,2,3,4} //example

#define VRAM_INIT_SEQ_1 {1,2,3,4} //example
#define VRAM_INIT_SEQ_2 {1,2,3,4} //example
                                  
#define GAMMA_CORR_INIT_SEQ_1 {1,2,3,4} //example

//gpio abstraction function implementations
#define _DC_INIT gpio_init(_ILI9341_DC)
#define _RST_INIT gpio_init(_ILI9341_RST)
#define _CS_INIT gpio_init(_ILI9341_CS)
#define _SCLK_INIT gpio_init(_ILI9341_SCLK)
#define _SDATA_INIT gpio_init(_ILI9341_SDATA)

#define _DC_SetDigitalOut gpio_set_dir(_ILI9341_DC, GPIO_OUT)
#define _RST_SetDigitalOut gpio_set_dir(_ILI9341_RST, GPIO_OUT)
#define _CS_SetDigitalOut gpio_set_dir(_ILI9341_CS, GPIO_OUT)
#define _SCLK_SetDigitalOut gpio_set_dir(_ILI9341_SCLK, GPIO_OUT)
#define _SDATA_SetDigitalOut gpio_set_dir(_ILI9341_SDATA, GPIO_OUT)

//MISO not used. keep in mind

#define _SCLK_SPI_FUNC gpio_set_function(_ILI9341_SCLK, GPIO_FUNC_SPI)
#define _SDATA_SPI_FUNC gpio_set_function(_ILI9341_SDATA, GPIO_FUNC_SPI)

//internal funcs
static void ili9341_writeCommand(uint8_t commandByte);
static void ili9341_writeData(uint8_t dataByte);
static void ili9341_writeData(uint8_t* dataByte, size_t len);

static void ili9341_hard_reset(); //bitbang reset

// 1 powerup & initialization routine
void ili9341_initialize(int8_t,int8_t,int8_t,int8_t,int8_t);

// 2 address window pointers
void ili9341_setAddrWindow(uint8_t,uint8_t,uint8_t,uint8_t);

// utility functions
void ili9341_drawFrame(uint32_t * buf, size_t len); //gp frame write; should be compatible with dma
void ili9341_write_565(uint32_t * bmpData, size_t len); //draw a bitmap from ram; optimized and shii


