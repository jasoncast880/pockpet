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

#ifdef __cplusplus
extern "C" {
#endif

extern Tile* tiles; //TODO: optimization/caching

//setup spi
void display_setup(); 
extern int cmd_chan;
extern int pixel_chan;
//
//pixel-buffer on heap via tile engine
extern uint16_t *pixel_buf_16;

enum cmd_sequence_t {
	CASET_CMD,
	CASET_DATA,
	RASET_CMD,
	RASET_DATA,
	RAMWR_CMD,
	PIX_BUF
};
void cmd_handler();

extern cmd_sequence_t tiling_state;

//runtime-related. (conceptual)
int draw_frame(Layer* layer); 

//need static-alloc buffers to hold the command params
static const uint8_t caset_cmd = static_cast<uint8_t>(CASET);
static uint8_t caset_params[4];

static const uint8_t raset_cmd = static_cast<uint8_t>(RASET);
static uint8_t raset_params[4];

static const uint8_t ramwr_cmd = static_cast<uint8_t>(RAM_WR);

#ifdef __cplusplus
}
#endif

#endif //DISPLAY_H
