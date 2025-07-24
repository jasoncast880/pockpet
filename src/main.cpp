#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"

#include "hardware/irq.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "secrets.h" //temporarily holds wifi creds & not pushed to git

#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 2) // is 0 + 2
#define MAIN_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE * 4)

#define NUM_BUTTONS 8
const uint BUTTON_PINS[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 8, 9};

QueueHandle_t button_queue;

void gpio_task_handler(uint gpio, uint32_t event_mask) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for(int i = 0; i<NUM_BUTTONS; i++) {
        if(gpio==BUTTON_PINS[i]){
            uint button_id = i;
            xQueueSendFromISR( button_queue, &button_id, &xHigherPriorityTaskWoken );
            break;
        }
    }
}

void buttons_handler_task( void *pvParameters ) {
    const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS( 500 );
    uint8_t button_id;

    for ( ;; ) {
        if(xQueueReceive(button_queue, &button_id, xMaxExpectedBlockTime)) {
            printf("Button %d pressed\n",button_id);
            //later use this to pass to global memory pool....
            //needs to be accesible in the other peripherals' code..
        } else {
            vTaskDelay(50);
        }
    } 
}

void setup(){ //irq setups, other peripheral setups


    for(int i = 0; i < NUM_BUTTONS; i++) {
        gpio_init(BUTTON_PINS[i]);
        gpio_set_dir(BUTTON_PINS[i], GPIO_IN);
        gpio_pull_up(BUTTON_PINS[i]);
        gpio_set_irq_enabled(
                BUTTON_PINS[i],
                GPIO_IRQ_EDGE_FALL,
                true);
    }

    irq_set_exclusive_handler(IO_IRQ_BANK0, gpio_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);


}

void main_task(void *pvParameters) {
    /*
    if (cyw43_arch_init()) {
        printf("Failed to init cyw43\n");
        while (1) tight_loop_contents();
    }

    printf("cyw43 arch init ok\n");  //BROKEN BROKEN BROKEN
    */

    for( ;; ){
        printf("periodic main task\n");
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main() {

    stdio_init_all();
    sleep_ms(5000);
    printf("GO\n");

    button_queue = xQueueCreate(10, sizeof(uint8_t));
    if(!button_queue) {
        printf("button queue no space :(\n");
        while(1);
    }

    setup(); //init button interrupts
             
    xTaskCreate(main_task, "main", 1000, NULL, 3, NULL );
    xTaskCreate(buttons_handler_task, "buttons", 1000, NULL, 1, NULL );

    
    vTaskStartScheduler();

    while (1) tight_loop_contents();
}

