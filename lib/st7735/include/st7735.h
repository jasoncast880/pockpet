#include "pico/stdlib.h"
#include "hardware/spi.h"

/*
 * st7735 shouuld control all of the reg hardware assignments and 
 * be an abstraction layer for all low level writes and commands..
 * although i will be writing app code in c
 * the driver should have a setup that can later be integrated into cpp
 */

#pragma once

//gp defines; 
//
//st7735 modes, reg addressing refer to strionix st7735 10.1 table

//gp regs
#define NOP 0x00
#define SWRESET 0x01
#define RDDID 0x04
#define RDDST 0x09

//modes

//???
#define RDDPM 0x0A
#define RDD_MADCTL 0x0B
#define RDD_COLMOD 0x0C
#define RDDIM 0x0D
#define RDDSM 0x0E

//modes
#define SLPIN 0x10
#define SLPOUT 0x11
#define PTLON 0x12
#define NORON 0x13
#define INVOFF 0x20
#define INVON 0x21
#define GAMSET 0x26
#define DISPOFF 0x28
#define DISPON 0x29

#define CASET 0x2A
#define RASET 0x2B
//there are more...

//frame rate control
//power control
//color control
//mem access data control registers

//gpio abstraction function implementations
#define _DC_INIT gpio_init(_ST7735_DC)
#define _RST_INIT gpio_init(_ST7735_RST)
#define _CS_INIT gpio_init(_ST7735_CS)
#define _SCLK_INIT gpio_init(_ST7735_SCLK)
#define _SDATA_INIT gpio_init(_ST7735_SDATA)

#define _DC_SetDigitalOut gpio_set_dir(_ST7735_DC, GPIO_OUT)
#define _RST_SetDigitalOut gpio_set_dir(_ST7735_RST, GPIO_OUT)
#define _CS_SetDigitalOut gpio_set_dir(_ST7735_CS, GPIO_OUT)
#define _SCLK_SetDigitalOut gpio_set_dir(_ST7735_SCLK, GPIO_OUT)
#define _SDATA_SetDigitalOut gpio_set_dir(_ST7735_SDATA, GPIO_OUT)

//MISO not used. keep in mind

#define _SCLK_SPI_FUNC gpio_set_function(_ST7735_SCLK, GPIO_FUNC_SPI)
#define _SDATA_SPI_FUNC gpio_set_function(_ST7735_SDATA, GPIO_FUNC_SPI)

//internal funcs
void writeCommand(uint8_t commandByte)
void writeData(uint8_t dataByte)

// 1 powerup & initialization routine
void st7735_initialize(int8_t,int8_t,int8_t,int8_t,int8_t);

// 2 address window pointers
void st7735_setAddrWindow(uint8_t,uint8_t,uint8_t,uint8_t);

// 3 implement spi_write_blocking to perform an spi write data buf
void st7735_spiWriteDataBuffer(uint8_t* spiData, uint32_t len);


