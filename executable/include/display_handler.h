//purpose of this file: 
// 1) basic LCD configuration, setup display using the repo's ili9341 HAL library
// 2) interface LCD Handler's structs, unique data types <=> ili9341 HAL & functionalities in a thread-safe way.
// 3) expose debugging-relevant variables to a gdb-friendly interface
//
// NOTE: 26-11-25: going to add dma handling to d quick buffer updates; MUST understand
// how to safely set isolated pins (ie dc, rst) in a thread safe way. How to 

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "FrameHandler.h"

#include "pinout.h"
#include "pico/stdlib.h"

#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/spi.h"

#include "ili9341.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#ifdef __cplusplus
extern "C" { 
#endif
	static SemaphoreHandle_t xDisplaySemaphore;
	extern QueueHandle_t xDisplayHandlerQueue;

	void lcd_render_task(void* pvParameters); 
	void lcd_write_task(void* pvParameters); 


	//dma handling: reserve a channel (of 12) for bulk spi transfers; ie for 2d image draws
	static int display_dma_transfer_sync(const void *buf, size_t size, TickType_t timeout); //rtos aware dma handler (BLOCKING)
#ifdef __cplusplus
}
#endif

void dma_irq_handler();
void display_setup();

#endif //DISPLAYHANDLER_H
