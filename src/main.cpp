#include <cstddef>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <stdio.h>
#include "class/cdc/cdc_device.h"
#include "modules/buttons/buttons.h"
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
		//blink();
	}
}

#include "usb.h"
void usb_task(void* pvParameters) {
	//usb_setup();

	tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

	for( ;; ) {
		tud_task();

		if( tud_cdc_connected() ) {
			tud_cdc_write_str("Hello World");
			tud_cdc_write_flush();
		}

		if( tud_cdc_available() ) {
			uint8_t buf[64]; 
			uint32_t ct = tud_cdc_read(buf, sizeof(buf));

			tud_cdc_write(buf, sizeof(buf));
			tud_cdc_write_flush();
		}
	}
}

#include "buttons.h"
#define BUTTON_SUPER_LATENCY_MS 10
void button_task(void* pvParameters) {
	button_setup();

	for( ;; ) {
		if(xButtonItem)
			xQueueSendToBack(xButtonQueue, &xButtonItem, BUTTON_SUPER_LATENCY_MS);
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

	sleep_ms(5000);
	//stdio_init_all();
	//printf("START");

	xTaskCreate( main_task, "main_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	//xTaskCreate( display_task, "display", 5000, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( usb_task, "usb_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( button_task, "buttons_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	vTaskStartScheduler();

	while(1) {
		tight_loop_contents();
	}
}

extern "C" {

	void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
		//printf("%s Task Stack Overflow failed\n", pcTaskName);
		while(1);
	}

	__attribute__((used)) void keep_heap_symbols(void) { 
		volatile size_t tmp;
		tmp = xPortGetFreeHeapSize();
		tmp = xPortGetMinimumEverFreeHeapSize();
	}

}
