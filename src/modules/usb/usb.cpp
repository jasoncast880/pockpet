#include "usb.h"

#include "bsp/board_api.h"
#include "class/cdc/cdc_device.h"
#include "projdefs.h"
#include "tusb.h"

/*
void tud_cdc_rx_cb(uint8_t itf) {
	uint8_t buf[64];
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));
}
*/

#ifdef RTOS_MODE

void usb_task(void* pvParameters) {
    board_init();
    tusb_init();

    // TinyUSB board init callback after init
    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    // let pico sdk use the first cdc interface for std io
    stdio_init_all();

	for( ;; ) {
		tud_task();

		if( tud_cdc_n_connected(0) ) {
			tud_cdc_write_str("Hello World");
			tud_cdc_write_flush();
            vTaskDelay(pdMS_TO_TICKS(5000));
		}
	}
}


// callback when data is received on a CDC interface
void tud_cdc_rx_cb(uint8_t itf)
{
    // allocate buffer for the data in the stack
    uint8_t buf[CFG_TUD_CDC_RX_BUFSIZE];

    printf("RX CDC %d\n", itf);

    // read the available data 
    // | IMPORTANT: also do this for CDC0 because otherwise
    // | you won't be able to print anymore to CDC0
    // | next time this function is called
    uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

    // check if the data was received on the second cdc interface
    if (itf == 1) {
        // process the received data
        buf[count] = 0; // null-terminate the string
        // now echo data back to the console on CDC 0
        printf("Received on CDC 1: %s\n", buf);

        // and echo back OK on CDC 1
        tud_cdc_n_write(itf, (uint8_t const *) "OK\r\n", 4);
        tud_cdc_n_write_flush(itf);
    }
}

#endif //RTOS MODE
