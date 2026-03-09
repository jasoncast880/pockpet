#include <cstddef>
#include <hardware/gpio.h>
#include <stdio.h>
#include "class/cdc/cdc_device.h"
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"

#include "projdefs.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

enum {
	BLINK_NOT_MOUNTED = 20,
	BLINK_MOUNTED = 1000,
	BLINK_SUSPENDED = 500,
};
static uint32_t BLINK_INTERVAL_MS = BLINK_NOT_MOUNTED;

//blink for status indicator
void main_task(void *pvParameters) {
	for( ;; ){
		vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
		gpio_put( PICO_DEFAULT_LED_PIN_INVERTED , true );
		vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
		gpio_put( PICO_DEFAULT_LED_PIN_INVERTED , false );
	}
}

#include "usb.h"
void usb_task(void* pvParameters) {
	usb_setup();

	for( ;; ) {
		vTaskDelay(pdMS_TO_TICKS(500));
		tud_task();

		if( tud_cdc_available() ) {
			uint8_t buf[64]; 
			uint32_t ct = tud_cdc_read(buf, sizeof(buf));
			//read buf, and see if there is dynamic updates. 
		}
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

	Tileset* jet_tileset = new Tileset( (uint16_t*)&jet_sprite_16[0], static_cast<size_t>(4096) );
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

		if(display.draw_dirty_tiles(screen) < 0 ) {
		    x--; //pos change for engine to chew on
    }
	}
}



int main() {

	stdio_init_all();
	sleep_ms(5000);
	printf("GO\n");

	xTaskCreate( main_task, "main", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( display_task, "display", 5000, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( usb_task, "usb", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	vTaskStartScheduler();

	while(1) {
		tight_loop_contents();
	}
}

extern "C" {

	void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
		printf("%s Task Stack Overflow failed\n", pcTaskName);
		while(1);
	}

	__attribute__((used)) void keep_heap_symbols(void) { 
		volatile size_t tmp;
		tmp = xPortGetFreeHeapSize();
		tmp = xPortGetMinimumEverFreeHeapSize();
	}

}
