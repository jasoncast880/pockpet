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

struct {
	uint16_t* buf;
	size_t size;
} display_item_t;

QueueHandle_t xDisplay_queue;
SemaphoreHandle_t xDisplay_mutex;

void display_setup();

int write_halfword(uint16_t hword); //for drawing a pixel ; SLOW
int write_block(uint16_t* buf, size_t size); //for drawing a pixel ; SLOW
void control_block_handler(); //for render to grab this.



//in order of priority...
void lcd_write(void* pvParameters); //hardware-facing
void lcd_render(void* pvParameters); //engine-facing

#ifdef __cplusplus
}
#endif

#endif //DISPLAY_H
