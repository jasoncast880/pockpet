#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "diskio.h"

#ifdef __cplusplus
extern "C"{
#endif


//HAL for the sdc portion of the screen hardware module
extern int8_t _SDC_CS;
extern int8_t _SDC_MOSI;
extern int8_t _SDC_MISO;
extern int8_t _SDC_SCLK;

//define cmd names
#define CMD0 0 //go idle state
#define CMD8 8 //send if condition
#define CMD12 12
#define CMD17 17 //read single block
#define CMD18 18 //read multiple block
#define CMD24 24 //write single block
#define CMD25 25 //write multiple block
#define CMD55 55 //app_cmd(whatever that menans)
#define CMD58 58 //read op-cond-reg (OCR)
#define ACMD41 41 //send OP con (app command)
                 

#define _CS_INIT gpio_init(_SDC_CS);
#define _MOSI_INIT gpio_init(_SDC_MOSI);
#define _MISO_INIT gpio_init(_SDC_MISO);
#define _SCLK_INIT gpio_init(_SDC_SCLK);

#define _CS_SetPinOut gpio_set_dir(_SDC_CS,GPIO_OUT);
#define _MOSI_SetPinOut gpio_set_dir(_SDC_MOSI,GPIO_OUT);
#define _SCLK_SetPinOut gpio_set_dir(_SDC_SCLK,GPIO_OUT);

#define _MISO_SetPinIn gpio_set_dir(_SDC_MISO,GPIO_IN);

//note pins for mosi, sclk are sharing same bus as the display

static void spi_select();
static void spi_deselect();
uint8_t sdc_send_command(uint8_t cmd, uint32_t arg);
bool sdc_init(int8_t cs, int8_t mosi, int8_t miso, int8_t sclk);
bool sdc_read(uint8_t *buf, uint32_t sector, uint32_t count);
bool sdc_write(uint8_t *buf, uint32_t sector, uint32_t count);
bool sdc_ioctl(uint32_t cmd, void* buf); //void ptr ; points to any data type

#ifdef __cplusplus
}
#endif
