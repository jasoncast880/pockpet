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

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DMA_DISPLAY_CH 0x00

enum cmd_sequence_t {
	CASET_CMD,
	CASET_DATA,
	RASET_CMD,
	RASET_DATA,
	RAMWR_CMD,
	PIX_BUF
};

void display_task( void* pvParameters ); //rtos, for rtos things. declare here, define in 
																				 //main.cpp (for now)

class DisplayHandler {
private:
	DisplayHandler(Layer* base);
	~DisplayHandler();
public:
	
	//TILE-RELATED - Run-Access within ISR.
	inline static DirtyTile* current_tile = nullptr;//iteration through Layer::dirty_tiles
	inline static DirtyTile* end_tile = nullptr;

	volatile inline static int dirty_flag = 1; 
	// -1, waiting reset
	// 0, done all tiles
	// 1, tile writing in prog.

	inline static uint8_t *pixel_buf_8 = nullptr;
  //inline static uint16_t *pixel_buf_16 = nullptr;

	inline static uint display_chan = DMA_DISPLAY_CH;
	inline static dma_channel_config cfg;
	
	//need static-alloc buffers to hold the command params
	inline static const uint8_t caset_cmd = static_cast<uint8_t>(CASET);
	inline static uint8_t caset_params[4];
	inline static const uint8_t raset_cmd = static_cast<uint8_t>(RASET);
	inline static uint8_t raset_params[4];
	inline static const uint8_t ramwr_cmd = static_cast<uint8_t>(RAM_WR);

	volatile inline static Layer* base_layer = nullptr;
	volatile inline static cmd_sequence_t tiling_state = CASET_CMD;
	//END OF TILE-RELATED

	static cmd_sequence_t state_fromISR(); //called within ISR ; slow

	static DisplayHandler& setup(Layer* base);
	DisplayHandler(const DisplayHandler& copy) = delete; 
	DisplayHandler& operator=(const DisplayHandler& copy) = delete; 

	int draw_clean_tiles(Layer* layer); 
	int draw_dirty_tiles(Layer* layer); 
};

void dma_handler(); //ISR ; DMA-Triggered

#ifdef __cplusplus
}
#endif

#endif //DISPLAY_H
