#include "usb.h"

#include "bsp/board_api.h"
#include "tusb.h"

void usb_setup() {
	board_init();

	tud_init(BOARD_TUD_RHPORT);

	if(board_init_after_tusb) {
		board_init_after_tusb();
	}

	//run some kind of var check? idk what tusb sets
}

