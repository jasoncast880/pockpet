#include "display.h"

#include <hardware/dma.h>
#include <tile_engine.h>
#include "ili9341.h"

//#ifdef RTOS_MODE

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "sync_common.h"

void display_task( void* pvParameters ) {
	//run base system tileset & map pointer initializer
	add_Layer(base-layer);
	add_Sprite(cursor-shi);
	
	//setup the dma channels for bulk processing
	dma_claim_mask(1u << 0);
	
	for(;;) {
		/*
		 * basic render cycle: 
		 * render via tile engine -> push data ->
		 * parse inputs from other modules into render commands -> repeat
		 */

		xSemaphoreTake(spi0_sync_t, portMAX_DELAY);

		update_sprite(id, x, y, map);
		eng_render();

		//crazy display state machine shi

		xSemaphoreGive(spi0_sync_t);
	}
}

//#endif 
