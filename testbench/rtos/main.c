#include <stdio.h>

#include "pico/stdlib.h"
//#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#include "lwip/netif.h"
#include "lwip/ip4_addr.h"

#include "FreeRTOS.h"
#include "task.h"

#include "network_handler.h"

// Which core to run on if configNUMBER_OF_CORES==1
#ifndef RUN_FREE_RTOS_ON_CORE
#define RUN_FREE_RTOS_ON_CORE 0
#endif

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void main_wifi_cnxn_task(__unused void* param) {
    printf("init wifi...\n");
    if (cyw43_arch_init()) {
        printf("cyw43 init failed\n");
        vTaskDelete(NULL);
    }

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(SSID, PWD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        vTaskDelete(NULL);
    } else {
        printf("Connected.\n");
    }

//    xTaskCreate(blink_task, "blink", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void main_task(__unused void *params) {
    /*
    async_context_t* context = network_context_init();
    async_context_add_at_time_worker_in_ms(context, &network_worker_timeout, 0); //what it do
    */
    xTaskCreate(main_wifi_cnxn_task, "wifi", 4096, NULL, BLINK_TASK_PRIORITY, NULL);
    int count = 0;
    while(1){
        printf("Hello from main, count = %d\n",count++);
        vTaskDelay(3000);
    }
}

void vLaunch( void) {
    TaskHandle_t main_task_h;
    xTaskCreate(main_task, "MainThread", 4096, NULL, MAIN_TASK_PRIORITY, &main_task_h);

#if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1 //not used??
    // we must bind the main task to one core (well at least while the init is called)
    vTaskCoreAffinitySet(main_task_h, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("Stack overflow in task: %s\n", pcTaskName);
    while (1); // halt for debugging
}

int main( void )
{
    
    stdio_init_all();

    sleep_ms(5000);
    printf("GO\n");

    vLaunch();
    return 0;
}
