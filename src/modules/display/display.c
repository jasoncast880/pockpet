#include "display.h"

#include <tile_engine.h>
#include "ili9341.h"

#ifdef RTOS_MODE

#include "sync_common.h"

void render_task( void* pvParameters ) {
	for(;;) {
		//parse system commands into render-able stuff
		//every loop do a render cycle
		//use c-wrapper functions to interact with the engine
	}
}

void display_task( void* pvParameters ) {
	for(;;) {
		//pure pusher
		//dma state machine, use linked list or smilar to address
		//blabla

	}
}

#endif 

/*
 * BLOCKERS:
 * Fix tile engine to be C-Compatible
 * diagrams on the tile engine accessible structures.
 *
 * revisit the driver and make it more reliable, consistent semantics
 */



