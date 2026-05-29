#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"
#include <stddef.h>
#include <stdint.h>

#include "pinout.h"       
#include "hardware/dma.h"
#include "hardware/irq.h"
#include <hardware/regs/intctrl.h>
#include <hardware/spi.h>
#include <pico/platform/common.h>

#include "ili9341.h"
#include "engine_api.h"

#define DMA_DISPLAY_CH 0x00

#ifdef __cplusplus
extern "C" {
#endif

enum cmd_sequence_t {
	CASET_CMD,
	CASET_DATA,
	RASET_CMD,
	RASET_DATA,
	RAMWR_CMD,
	PIX_BUF
}; //this is a little outdated but its good reference to remember
   //the driver sequence without looking at the DS

void display_setup();

void tile_handler(); //for tile-by-tile PARTIAL rendering ISR
void frame_handler(); //for full framebuffer rendering    ISR
void hscanline_handler(); //for full framebuffer rendering    ISR

//user-app things
static Engine* e; //software

#ifdef RTOS_MODE

#include <FreeRTOS.h>
#include "task.h"
#include "semphr.h"

#include "sync_common.h"

SemaphoreHandle_t render_token;

void push_pixels( void* pvParameters );
void render( void* pvParameters );
void update_entities( void* pvParams );
#endif
#ifndef RTOSMODE 

static bool render_flag;
#endif


#ifdef __cplusplus
} //extern "C"
#endif

#endif //DISPLAY_H
