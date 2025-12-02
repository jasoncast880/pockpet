//purpose of this file: 
// 1) basic LCD configuration, setup display using the repo's ili9341 HAL library
// 2) interface LCD Handler's structs, unique data types <=> ili9341 HAL & functionalities in a thread-safe way.
// 3) expose debugging-relevant variables to a gdb-friendly interface
//
// NOTE: 26-11-25: going to add dma handling to d quick buffer updates; MUST understand
// how to safely set isolated pins (ie dc, rst) in a thread safe way. How to 

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "pinout.h"
#include "pico/stdlib.h"

#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/spi.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" { 
#endif

	struct display_msg_t { //members of the render class
	std::vector<Tile> *renderedTiles;
	std::vector<uint8_t> *indexList;
	size_t size;
	}

	//init these on setup
	static SemaphoreHandle_t xDisplaySemaphore; //for syncing the two tasks' ram access
	static QueueHandle_t xDisplayQueue;


	/*!!!---RENDER TASK---!!!*/
	Tileset* sys_tileset;
	Tileset* jet_tileset;

	Scene* base;
	Sprite* jetsprite;
	RenderController* render;

	uint8_t* maps[4] = {&demo_spritemap_1[0], &demo_spritemap_2[0], &demo_spritemap_3[0], &demo_spritemap_4[0]};

	void lcd_render_task(void* pvParameters); 
	/*-----------------------*/


	/*!!!---WRITE TASK---!!!*/
	static int dma_chan;
	static SemaphoreHandle_t dma_spi0_smphr; //this will have to change based on spi bus
	static spi_inst_t* driver_spi;

	static int display_dma_transfer_blocking(const void *buf, size_t size, TickType_t timeout); //rtos aware dma handler (BLOCKING)
	static void dma_irq_handler();

	void lcd_write_task(void* pvParameters); 
	/*-----------------------*/

#ifdef __cplusplus
}
#endif

void display_setup();

#endif //DISPLAYHANDLER_H
