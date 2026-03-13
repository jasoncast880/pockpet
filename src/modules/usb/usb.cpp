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


/*
void tud_cdc_rx_cb(uint8_t itf) {
	uint8_t buf[64];
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));
}
*/

#ifdef RTOS_MODE

void usb_task(void* pvParameters) {
	//usb_setup();

	tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

	for( ;; ) {
		tud_task();

		if( tud_cdc_connected() ) {
			tud_cdc_write_str("Hello World");
			tud_cdc_write_flush();
		}

		if( tud_cdc_available() ) {
			uint8_t buf[64]; 
			uint32_t ct = tud_cdc_read(buf, sizeof(buf));

			tud_cdc_write(buf, sizeof(buf));
			tud_cdc_write_flush();
		}
	}
}

#endif //RTOS_MODE

void usb_task(void* pvParameters) {
	//usb_setup();

	tusb_rhport_init_t dev_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUSB_SPEED_AUTO
  };
  tusb_init(BOARD_TUD_RHPORT, &dev_init);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

	for( ;; ) {
		tud_task();

		if( tud_cdc_connected() ) {
			tud_cdc_write_str("Hello World");
			tud_cdc_write_flush();
		}

		if( tud_cdc_available() ) {
			uint8_t buf[64]; 
			uint32_t ct = tud_cdc_read(buf, sizeof(buf));

			tud_cdc_write(buf, sizeof(buf));
			tud_cdc_write_flush();
		}
	}
}

