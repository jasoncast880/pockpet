#include "usb.h"

#include "FreeRTOSConfig.h"
#include "bsp/board_api.h"
#include "class/cdc/cdc_device.h"
#include "portmacro.h"
#include "projdefs.h"
#include <string>
#include "tusb.h"

#include <pico/platform/panic.h>

#ifdef RTOS_MODE

typedef void (*cmd_fn)(int argc, char** argv);

typedef struct {
	const char* cmd;
	cmd_fn fn;
} command_t;

command_t commands[] = {
	{ "banner", banner },
	{ "demo_1", demo_display }
};

xSemaphoreHandle rx_flag = xSemaphoreCreateMutex();
static char line_buf[128];
static uint32_t line_len = 0;

std::string title_banner = R"(    ___       ___       ___       ___       ___       ___       ___       ___       ___       ___    
    /\  \     /\  \     /\  \     /\  \     /\  \     /\  \     /\  \     /\  \     /\__\     /\  \  
    \:\  \   /::\  \   /::\  \    \:\  \   _\:\  \   /::\  \   /::\  \   /::\  \   /:/__/_   _\:\  \ 
    /::\__\ /::\:\__\ /:/\:\__\   /::\__\ /\/::\__\ /:/\:\__\ /::\:\__\ /:/\:\__\ /::\/\__\ /\/::\__\
   /:/\/__/ \/\::/  / \:\ \/__/  /:/\/__/ \::/\/__/ \:\:\/__/ \/\::/  / \:\ \/__/ \/\::/  / \::/\/__/
   \/__/      /:/  /   \:\__\    \/__/     \:\__\    \::/  /    /:/  /   \:\__\     /:/  /   \:\__\  
              \/__/     \/__/               \/__/     \/__/     \/__/     \/__/     \/__/     \/__/  )";
uint32_t row_len = 102;

void banner(int argc, char** argv) {
	uint32_t banner_len = sizeof(title_banner)/sizeof(char);

	char* p = &title_banner[0];
	for( int i = 0 ; i < (banner_len/row_len) ; i++ ) {
		for( int j = 0 ; j < row_len ; j++) {
			char c = title_banner[i*row_len + j];
			tud_cdc_n_write_char(0, c);
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

	if(itf==0) {
		for ( uint32_t i = 0 ; i<count ; i++ ) {
			char c = buf[i];

			if( c=='\n' || c=='\r' ) {

				line_buf[line_len] = '\0'; //null terminator
				tud_cdc_write_str("\r\n"); 
				line_len = 0;

				xSemaphoreGiveFromISR(rx_flag, pdFALSE);

			} else {
				if( line_len<sizeof(line_buf)-1 ) {
					line_buf[line_len++] = c;
				}

				tud_cdc_write_char(c);
				tud_cdc_write_flush();
			}
		}
	}
}

extern "C" {

	void usb_task(void* pvParameters) {
		xTaskCreate(shell_task, "shell_daemon", (configMINIMAL_STACK_SIZE * 4) , NULL,  tskIDLE_PRIORITY, NULL); //TEST THIS
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

	void shell_task(void* pvParameters) {

		static char cmd_str[128];

		for(;;) {
			if( xSemaphoreTake(rx_flag, pdMS_TO_TICKS(100)) ==pdTRUE ) {

				char* c = &line_buf[0];
				int i = 0;

				while( *c!='\0') {
					cmd_str[i] = *c;
					i++;
					c++;
				}

				if( strncmp( &cmd_str[0], "banner", 6 ) == 0 ) {
					banner(0,0);
				} else if(strncmp( &cmd_str[0], "demo_1", 6 )) {
				}

				xSemaphoreGive(rx_flag);
			}
		}
	}

}

#endif //RTOS MODE
