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

enum cmd_sequence_t {
	CASET_CMD,
	CASET_DATA,
	RASET_CMD,
	RASET_DATA,
	RAMWR_CMD,
	PIX_BUF
};

class DisplayHandler {
private:
	DisplayHandler(Layer* base);
	~DisplayHandler();
public:
	//hw resources-related
	inline static int cmd_chan = 0;
	inline static int pixel_chan = 0;
	inline static uint16_t *pixel_buf_16 = nullptr;

	//need static-alloc buffers to hold the command params
	inline static const uint8_t caset_cmd = static_cast<uint8_t>(CASET);
	inline static uint8_t caset_params[4];

	inline static const uint8_t raset_cmd = static_cast<uint8_t>(RASET);
	inline static uint8_t raset_params[4];

	inline static const uint8_t ramwr_cmd = static_cast<uint8_t>(RAM_WR);

	//tiling trackers
	
	
	static DisplayHandler& setup(Layer* base); //enforce singleton
	DisplayHandler(const DisplayHandler& copy) = delete; //enforce singleton
	DisplayHandler& operator=(const DisplayHandler& copy) = delete; //enforce singleton

	//runtime-related. (conceptual)
	int draw_frame(Layer* layer); 

	inline static Layer* base_layer = nullptr;
	inline static cmd_sequence_t tiling_state = PIX_BUF;
	static cmd_sequence_t state_fromISR(cmd_sequence_t state);
};

void cmd_handler(); 

#ifdef __cplusplus
}
#endif

#endif //DISPLAY_H
