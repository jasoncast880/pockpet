#include "button_handler.h"

void button_setup() {

    irq_set_exclusive_handler(IO_IRQ_BANK0, gpio_irq_handler);
    irq_set_enabled(IO_IRQ_BANK0, true);

    for(int i = 0; i < NUM_BUTTONS; i++) {
        gpio_init(BUTTON_PINS[i]);
        gpio_set_dir(BUTTON_PINS[i], GPIO_IN);
        gpio_pull_up(BUTTON_PINS[i]);
        gpio_set_irq_enabled(
                BUTTON_PINS[i],
                GPIO_IRQ_EDGE_FALL,
                true);
    }

    xButtonQueue = xQueueCreate(10, sizeof(uint8_t));
    if(!xButtonQueue) {
        while(1);
    }

    printf("button_setup OK\n");
}

void gpio_irq_handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    for( uint gpio = 2; gpio<9; gpio++) {
        uint32_t events = gpio_get_irq_event_mask(gpio);
        if(events & GPIO_IRQ_EDGE_FALL){
            gpio_acknowledge_irq(gpio, GPIO_IRQ_EDGE_FALL); //acknowledg this does not account for the button hold; only for press

            xQueueSendFromISR(xButtonQueue, &gpio, &xHigherPriorityTaskWoken);
        }
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void buttons_queue_task( void *pvParameters ) { 
    const TickType_t xMaxExpectedBlockTime = pdMS_TO_TICKS( 500 );

    uint8_t button_id;
    uint32_t time_last_pressed[9] = {0};

    for ( ;; ) {
        if(xQueueReceive(xButtonQueue, &button_id, xMaxExpectedBlockTime)) {
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

