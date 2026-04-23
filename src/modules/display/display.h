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
};

void display_setup();

void tile_handler();

#ifdef RTOS_MODE
#include "sync_common.h"
void display_task( void* pvParameters ); 

}
#endif


#ifdef __cplusplus
} //extern "C"
#endif

#endif //DISPLAY_H
