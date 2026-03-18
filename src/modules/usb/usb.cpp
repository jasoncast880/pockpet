#include "usb.h"

#include "bsp/board_api.h"
#include "class/cdc/cdc_device.h"
#include "projdefs.h"
#include "tusb.h"

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

        /*
		if( tud_cdc_n_connected(0) ) {
			tud_cdc_write_str("Hello World");
			tud_cdc_write_flush();
            vTaskDelay(pdMS_TO_TICKS(5000));
		}
        */
        vTaskDelay(pdMS_TO_TICKS(1));
	}
}

static char line_buf[128];
static uint32_t line_len = 0;

void tud_cdc_rx_cb(uint8_t itf)
{
    uint8_t buf[64];
    uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

    for ( uint32_t i = 0 ; i<count ; i++ ) {
        char c = buf[i];

        if( c=='\n' || c=='\r' ) {

            line_buf[line_len] = '\0';

            tud_cdc_write(line_buf, line_len);
            tud_cdc_write_str("\r\n");
            tud_cdc_write_flush();

            line_len = 0;
        } else {
            if( line_len<sizeof(line_buf)-1 ) {
                line_buf[line_len++] = c;
            }

            tud_cdc_write_char(c);
            tud_cdc_write_flush();
        }
    }
}

#endif //RTOS MODE
