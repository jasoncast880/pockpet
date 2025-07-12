#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

//drivers below: they hold the tasks and stuff
//#include "displayHandler.h"
//#include "sdcHandler.h"
#include "networkHandler.h"

#include "FreeRTOS.h"
#include "task.h"

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define WORKER_TASK_PRIORITY    ( tskIDLE_PRIORITY + 4UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 4)
#define WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void main_task(__unused void *params) {

    int count = 0; 

    while(true) {
        printf("Hello from main task count=%u\n", count++);
        vTaskDelay(3000);
    }
}

void peripherals_init() {
    WifiHandler wifi;
    wifi.network_handler_init(); 
    //sdc_handler_init();
    //...
}

void vLaunch(void) {

    peripherals_init();

    xTaskCreate(main_task, "Main Task", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);
    vTaskStartScheduler();

}

int main( void )
{
    stdio_init_all();

    sleep_ms(10000);
    printf("GO\n");
    printf("Starting  on core 0:\n");

    vLaunch();

    while(1) { tight_loop_contents(); }

    return 0;
}
