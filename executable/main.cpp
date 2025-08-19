#include <stdio.h>
#include "pico/stdlib.h"

/* wifi is bad!!!!!
#include "pico/cyw43_arch.h"
#include "lwip/ip4_addr.h"
*/

#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "display_handler.h"

#include "secrets.h" //temporarily holds wifi creds & not pushed to git

#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 2) // is 0 + 2
#define MAIN_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE * 4)

#define NUM_BUTTONS 8
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {2, 3, 4, 5, 6, 7, 8, 9};

QueueHandle_t button_queue;

void gpio_irq_handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for( uint gpio = 2; gpio<9; gpio++) {
        uint32_t events = gpio_get_irq_event_mask(gpio);
        if(events & GPIO_IRQ_EDGE_FALL){
            gpio_acknowledge_irq(gpio, GPIO_IRQ_EDGE_FALL); //acknowledg this does not account for the button hold; only for press

            xQueueSendFromISR(button_queue, &gpio, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void buttons_queue_task( void *pvParameters ) { //new improved version of 'buttons_handler_task'..
    const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS( 500 );

    uint8_t button_id;
    uint32_t time_last_pressed[9] = {0};

    for ( ;; ) {
        if(xQueueReceive(button_queue, &button_id, xMaxExpectedBlockTime)) {
            uint32_t now = xTaskGetTickCount();
            if( (now-time_last_pressed[button_id]) >= pdMS_TO_TICKS(20) ) {
                printf("%d ", button_id);
                time_last_pressed[button_id] = now;
            } else { 
                //disregard
            }
        } else { 
            printf("\n");
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

void sdc_handler_task(void* pvParameters) { }

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

Base base;
Sprite sprites[10]; //10 inst max

SemaphoreHandle_t xDisplaySemaphore; //signal for render-write loop

int main() {

    stdio_init_all();
    sleep_ms(5000);
    printf("GO\n");

    //do the initializatation video seq here

    button_queue = xQueueCreate(10, sizeof(uint8_t));
    if(!button_queue) {
        printf("button queue no space :(\n");
        while(1);
    }

    setup(); //init button interrupts
             
    xTaskCreate( main_task, "main", 1000, NULL, 3, NULL );

    //defined in display_handler file
    xTaskCreate( lcd_write_task, "lcd_write_task", 2000, NULL, 2, NULL );
    xTaskCreate( lcd_render_task, "lcd_render_task", 2000, NULL, 2, NULL );

    xTaskCreate( buttons_queue_task, "buttons", 1000, NULL, 1, NULL );
    
    vTaskStartScheduler();

    while (1) tight_loop_contents();
}
