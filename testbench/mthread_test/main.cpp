#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

//drivers below: they hold the tasks and stuff
//#include "displayHandler.h"
//#include "sdcHandler.h"
//#include "networkHandler.h"

#include "FreeRTOS.h"
#include "task.h"

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 8)
#define WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void peripherals_init() {
    WifiHandler wifi;
    wifi.network_handler_init(); 
    //sdc_handler_init();
    //...
}

void main_task(__unused void *params) {

    int count = 0; 

    peripherals_init();

    while(true) {
        printf("Hello from main task count=%u\n", count++);
        vTaskDelay(3000);
    }
}

void vLaunch(void) {

    printf("ok1\n");
    xTaskCreate(main_task, "Main Task", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);

    printf("ok2\n");
    vTaskStartScheduler();

}

int main( void )
{
    stdio_init_all();

    sleep_ms(10000);
    printf("GO\n");
    printf("Starting  on core 0:\n");

    if (cyw43_arch_init()) {
        printf("cyw43_arch_init() failed!\n");
        while (true) tight_loop_contents(); 
    }
    printf("arch init ok \n");

    cyw43_arch_enable_sta_mode();  
    printf("arch enable sta-mode ok \n");

    vLaunch();

    while(1) { tight_loop_contents(); }

    return 0;
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // Optional: print task name
    printf("Stack overflow in task: %s\n", pcTaskName);

    // Optional: halt system
    while (true) {
        __asm volatile ("bkpt #0");  // break into debugger if attached
    }
}
