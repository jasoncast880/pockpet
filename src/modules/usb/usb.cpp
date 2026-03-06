#include "usb.h"

#include "bsp/board_api.h"
#include "class/cdc/cdc_device.h"
#include "tusb.h"

void usb_setup() {
	board_init();

	tud_init(BOARD_TUD_RHPORT);

	if(board_init_after_tusb) {
		board_init_after_tusb();
	}
}

// now if you are using an rtos, this can become a real task
void usb_task() {
	tud_task();
}

void tud_cdc_tx_complete_cb(uint8_t itf) {
	uint8_t buf[64];
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

	if(itf==0) {}
	if(itf==2) {}
 
}

void tud_cdc_rx_complete_cb(uint8_t itf) {
	uint8_t buf[64];
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

	if(itf==1) {}
	if(itf==3) {}
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
}
