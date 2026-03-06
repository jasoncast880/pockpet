#include <cstddef>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"

#include "projdefs.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"


#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 2) // is 0 + 2
#define MAIN_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE * 4)

//globs
QueueHandle_t xButtonQueue = NULL;
QueueHandle_t xDisplayHandlerQueue = NULL;

//blink for status indicator
void main_task(void *pvParameters) {

	for( ;; ){
	}
}

#include "display.h"
#include "tile_engine.h"
#include <ampalaya_tileset_16.h>
#include <tilemaps.h>
#include <jet_sprite.h>

void display_task(void* pvParameters) {

	Tileset* sys_tileset = new Tileset( (uint16_t*)&ampalaya_tileset_16[0], static_cast<size_t>(DEFAULT_TILE_LEN*DEFAULT_TILE_LEN*30) );
	Layer* screen = new Layer(
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_X),
		static_cast<uint8_t>(DEFAULT_SCREEN_TILES_Y),
		sys_tileset,
		&tile_bg_16[0],
		0
		); //id not relevant yet ? TODO: id handling system.

	Tileset* jet_tileset = new Tileset((uint16_t*)&jet_sprite_16[0],static_cast<size_t>(4096));
	uint8_t cursor = screen->sprite_add(
		2,
		2,
		jet_tileset,
		&demo_spritemap_1[0]
	);

	DisplayHandler& display = DisplayHandler::setup(screen); 

	static int x = 100;
	static int y = 100;

	for( ;; ) {
		vTaskDelay(pdMS_TO_TICKS(500));

		screen->sprite_update_by_id(cursor, x, y, demo_spritemap_1);
		screen->render();

		if(display.draw_dirty_tiles(screen) < 0 )
			__breakpoint; //return 0; //note this has to return eventually
	
		x--; //pos change for engine to chew on
	}
}

void usb_task(void* pvParameters) {

	for( ;; ) {
		tight_loop_contents();
	}
}

int main() {

	stdio_init_all();
	sleep_ms(5000);
	printf("GO\n");


	xTaskCreate( main_task, "main", 1000, NULL, 3, NULL );
	xTaskCreate( display_task, "display", 1000, NULL, 3, NULL );//change to accom. stack
	xTaskCreate( usb_task, "usb", 1000, NULL, 3, NULL );
	vTaskStartScheduler();


	while(1) {
		tight_loop_contents();
	}
}

extern "C" { //hooks and stuff
#include "FreeRTOS.h"
#include "task.h"

	void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
		printf("%s Task Stack Overflow failed\n", pcTaskName);
		while(1);
	}

	__attribute__((used)) void keep_heap_symbols(void) {
		// Volatile cast prevents the compiler from optimizing the calls away
		volatile size_t tmp;
		tmp = xPortGetFreeHeapSize();
		tmp = xPortGetMinimumEverFreeHeapSize();
	}
}
