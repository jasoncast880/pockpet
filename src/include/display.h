#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"
#include <cstddef>
#include <stdint.h>

#include "pinout.h"       
#include "hardware/dma.h"
#include "hardware/irq.h"
#include <hardware/regs/intctrl.h>
#include <hardware/spi.h>
#include <pico/platform/common.h>

#include "ili9341.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "FrameHandler.h" 

#ifdef __cplusplus //TASKS, RTOS THINGS
extern "C" { 
#endif


typedef struct {
	uint32_t CB_NUM_TILES; //if you need to do tiles. Rep. how many tiles
	uint16_t* buf; //first pixel
	size_t size; //num pixels total OR Pixels/tile
} display_item_t;

QueueHandle_t xDisplayQueue;
SemaphoreHandle_t xDisplayMutex;

void display_setup();

//in order of priority...
void lcd_write(void* pvParameters); 

int data_chan;
int cfg_write_block(display_item_t block); // return a dma chan.
void dma_tiling_isr();

void lcd_render(void* pvParameters); //engine-facing

#ifdef __cplusplus
}
#endif

#endif //DISPLAY_H
