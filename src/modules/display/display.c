#include "display.h"

#include "pinout.h"
#include "sync_common.h"
#include "tile_engine.h"
#include "usb.h"

void display_setup() {
	ili9341_initialize(ILI9341_CS, ILI9341_RST, ILI9341_DC);
}

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "semphr.h"

void display_task( void* pvParameters ) {
	
	xTaskCreate(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t uxStackDepth, void *const pvParameters, UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask)
	for(;;) {
		
	}

}

//engine.h has pre-defined constraints: 
//number of layers max, number of sprites/layer.
//each layer assume a tileset of max 30 tiles, sprites are max of 10? tiles
//guesstimate the amount of ram that this can eat up. perhaps double of what's initially allocated, in order to make some space for the dirty tiles, various paddings, etc.

#include "ampalaya_tileset_16.h"
#include "jet_sprite.h"
#include "tilemaps.h"

void soft_render( void* pvParameters ) {
	add_layer(&amapalaya_tileset[0], 32, &tile_bg_16[0], DEFAULT_SCREEN_TILES_X, DEFAULT_SCREEN_TILES_Y );
	add_layer(&jet_sprite_16[0], 16, &demo_spritemap_1[0], 2 ,2 )
	for(;;) {
		//cursor modifier/input parsing via queues
		vTaskDelay()
	}
}
