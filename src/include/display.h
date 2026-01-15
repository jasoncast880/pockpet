#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"
#include <cstddef>
#include <stdint.h>

#include "pinout.h"       
#include "hardware/dma.h"
#include "hardware/irq.h"
#include <hardware/regs/intctrl.h>
#include <hardware/spi.h>
#include <pico/platform/common.h>

#include "ili9341.h"

#include "tile_engine.h" 

Tile* tiles; //TODO: optimization/caching

//setup spi
void display_setup(); 
int cmd_chan;
int pixel_chan;

//runtime-related. (conceptual)
int frame_draw(); 

//tiling isr (one channel for screen configuration, one for data/pixel throughput)
void tile_handler();
void cmd_handler();

//need static-alloc buffers to hold the command params
static const uint8_t caset_cmd = (uint8_t)CASET;
static uint8_t caset_params[4];

static const uint8_t raset_cmd = (uint8_t)RASET;
static uint8_t raset_params[4];

static const uint8_t ramwr_cmd = (uint8_t)RAM_WR;

//pixel-buffer on heap via tile engine
uint16_t *pixel_buf_16;

enum cmd_sequence_t {
	CASET_CMD = 0,
	CASET_DATA,
	RASET_CMD,
	RASET_DATA,
	RAMWR_CMD,
	//PIX_BUF
};//note this is evaulated after a channel finishes.

cmd_sequence_t tiling_state = CASET_CMD;

#endif //DISPLAY_H
