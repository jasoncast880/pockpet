#include <stdio.h>

#include "pico/stdlib.h"
//#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"

// Which core to run on if configNUMBER_OF_CORES==1
#ifndef RUN_FREE_RTOS_ON_CORE
#define RUN_FREE_RTOS_ON_CORE 0
#endif

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )
#define WORKER_TASK_PRIORITY    ( tskIDLE_PRIORITY + 4UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#include "pico/async_context_freertos.h"

static async_context_freertos_t async_context_instance;
static async_context_t* context_init(void) {
    async_context_freertos_config_t config = async_context_freertos_default_config();
    config.task_priority = WORKER_TASK_PRIORITY;
    config.task_stack_size = WORKER_TASK_STACK_SIZE;
    if(!async_context_freertos_init(&async_context_instance, &config))
        return NULL;
    return &async_context_instance.core;
}

void led_task(void *param) {
    if (cyw43_arch_init()) vTaskDelete(NULL);

    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

static void do_work(async_context_t *context, async_at_time_worker_t *worker) {
    async_context_add_at_time_worker_in_ms(context, worker, 10000);
    static uint32_t count = 0;
    printf("Hello from worker count=%u\n", count++);
}
async_at_time_worker_t worker_timeout = { .do_work = do_work };

void main_task(__unused void *params) {

    async_context_t* context = context_init();
    async_context_add_at_time_worker_in_ms(context, &worker_timeout, 0); //what it do
    xTaskCreate(led_task, "LED", 256, NULL, BLINK_TASK_PRIORITY, NULL);
    int count = 0;
    while(1){
        printf("Hello from main, count = %d\n",count++);
        vTaskDelay(3000);
    }
}

void vLaunch( void) {
    TaskHandle_t main_task_h;
    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &main_task_h);

#if configUSE_CORE_AFFINITY && configNUMBER_OF_CORES > 1 //not used??
    // we must bind the main task to one core (well at least while the init is called)
    vTaskCoreAffinitySet(main_task_h, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main( void )
{
    
    stdio_init_all();

    sleep_ms(3000);
    printf("GO\n");

    vLaunch();
    return 0;
}
