#include <cstddef>
#include <pico/stdio.h>
#include <stdio.h>
#include "pico/stdlib.h"

enum {
	BLINK_NOT_MOUNTED = 20,
	BLINK_MOUNTED = 1000,
	BLINK_SUSPENDED = 500,
};
static uint32_t BLINK_INTERVAL_MS = BLINK_NOT_MOUNTED;

#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"

void main_task(void *pvParameters) {
	hard_assert(cyw43_arch_init() == PICO_OK);
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
	for( ;; ){
		vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
		vTaskDelay(pdMS_TO_TICKS(BLINK_INTERVAL_MS));
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

	}
}
#include "buttons.h"
#include "display.h"
#include "usb.h"
#include "sync_common.h"

int main() {

	sleep_ms(5000);
    /*
	stdio_init_all();
	printf("START");
    */

	spi0_sync_t = xSemaphoreCreateBinary();

	xTaskCreate( main_task, "main_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
	xTaskCreate( usb_task, "usb_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );
//	xTaskCreate( display_task, "display", 5000, NULL, tskIDLE_PRIORITY+1, NULL );
//	xTaskCreate( button_task, "buttons_task", (configMINIMAL_STACK_SIZE * 4) , NULL, tskIDLE_PRIORITY+1, NULL );

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
