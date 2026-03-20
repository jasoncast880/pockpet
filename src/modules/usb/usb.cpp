#include "usb.h"

#include "bsp/board_api.h"
#include "class/cdc/cdc_device.h"
#include "projdefs.h"
#include "tusb.h"

#include <pico/platform/panic.h>

#ifdef RTOS_MODE

static char line_buf[128];
static uint32_t line_len = 0;

static SemaphoreHandle_t shell_flag = xSemaphoreCreateBinary();
QueueHandle_t usb_q;

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
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}

static struct cmd_struct {
	const char* cmd;
	(void)* opt;
};

static struct cmd_struct commands[] = {
	{ "banner", NULL },
	{ "demo_1" }
}

QueueHandle_t cmd_queue = xQueueCreate(1, sizeof(cmd_struct));

void shell_task(void* pvParameters) {
	struct cmd_struct cs;
	std::String dummy = "null";

	for(;;) {
		xQueueReceive( cmd_queue, &cs, pdMS_TO_TICKS(100));
		char* cmd = cs.cmd;

		if (cmd.equals("banner")) {
			banner();	
		} else if (cmd.equals("demo")) {
			demo_display();
		}
	}
}

int banner() {
	uint32_t size = sizeof(banner);

	char* p = banner[0];
	for( int i = 0 ; i < (size/row_size) ; i++ ) {
		for( int j = 0 ; j < row_size ; j++) {
			char c = *p;
			if( c!='\0' || c!= 'p') {
				tud_cdc_n_write_char(0, c);
			}
		}
		tud_cdc_n_write_str(0, "\r\n");
		tud_cdc_write_flush();
	}

	tud_cdc_n_write_str(0, "\r\n");
	tud_cdc_write_flush();

}

void tud_cdc_rx_cb(uint8_t itf) {
	uint8_t buf[64];
	uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

	for ( uint32_t i = 0 ; i<count ; i++ ) {
		char c = buf[i];

		if( c=='\n' || c=='\r' ) {

			line_buf[line_len] = '\0'; //null terminator

			tud_cdc_write_str("\r\n"); 
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

void tud_cdc_tx_complete_cb(uint8_t itf) {
	xSemaphoreGiveFromISR(xSemaphore, pxHigherPriorityTaskWoken);
}

#endif //RTOS MODE
