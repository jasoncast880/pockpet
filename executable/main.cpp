#include <cstddef>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pinout.h"

#include "FreeRTOS.h"
#include "sdc_handler.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

//task handlers here
#include "display_handler.h"
#include "button_handler.h"

#include "secrets.h" //temporarily holds wifi creds & not pushed to git

#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 2) // is 0 + 2
#define MAIN_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE * 4)

//globs
QueueHandle_t xButtonQueue = NULL;
QueueHandle_t xDisplayHandlerQueue = NULL;

//def in display handler
//Scene* base;
//
                  
void setup(){

    static SemaphoreHandle_t xSPI0_MUTEX = xSemaphoreCreateMutex();
    spi_init(spi0, 8000 * 1000); //spi freq @ 8Mhz
                                 
    gpio_set_function(SPI0_SCLK, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI0_TX, GPIO_FUNC_SPI);
    //

    button_setup();

    //spi0 bus users
    display_setup();
    sdc_setup();

}

//application code for now runs through here
void main_task(void *pvParameters) {

    //process button queue, and convert the user input into system commands.
    //ie; press up and sprite moves up
    //
    for( ;; ){
        printf("periodic main task\n");

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

//to scaffold: 
// file integrity tester
// sound .wav file buffering system

int main() {

    stdio_init_all();
    sleep_ms(5000);
    printf("GO\n");

    //do the initializatation video seq here

    setup(); 
             
    xTaskCreate( main_task, "main", 1000, NULL, 3, NULL );
    
    xTaskCreate( lcd_write_task, "lcd_write_task", 2000, NULL, 2, NULL );
    xTaskCreate( lcd_render_task, "lcd_render_task", 2000, NULL, 2, NULL );

    xTaskCreate( buttons_queue_task, "buttons", 1000, NULL, 3, NULL );
    
    vTaskStartScheduler();

    while (1) tight_loop_contents(); 

}

extern "C" { //hooks and stuff
#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName ) {
    printf("%s Task Stack Overflow failed\n", pcTaskName);
    while(1);
}

__attribute__((used)) void keep_heap_symbols(void) {
    // Volatile cast prevents the compiler from optimizing the calls away
    volatile size_t tmp;
    tmp = xPortGetFreeHeapSize();
    tmp = xPortGetMinimumEverFreeHeapSize();
}
}
