#include <cstddef>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <stdio.h>
#include "class/cdc/cdc_device.h"
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"

#include "projdefs.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

enum {
	BLINK_NOT_MOUNTED = 20,
	BLINK_MOUNTED = 1000,
	BLINK_SUSPENDED = 500,
};
static uint32_t BLINK_INTERVAL_MS = BLINK_NOT_MOUNTED;

//blink for status indicator
void main_task(void *pvParameters) {
	for( ;; ){
		vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
		//blink();
	}
}

#include "buttons.h"
#include "display.h"
#include "usb.h"

int main() {

	sleep_ms(5000);
	//stdio_init_all();
	printf("START");

	xTaskCreate( main_task, "main_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( display_task, "display", 5000, NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( usb_task, "usb_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( button_task, "buttons_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );

	vTaskStartScheduler();

	while(1) {
		tight_loop_contents();
	}
}

extern "C" {

	void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
		printf("%s Task Stack Overflow failed\n", pcTaskName);
		while(1);
	}

	__attribute__((used)) void keep_heap_symbols(void) { 
		volatile size_t tmp;
		tmp = xPortGetFreeHeapSize();
		tmp = xPortGetMinimumEverFreeHeapSize();
	}

}
