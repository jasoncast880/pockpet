#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
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
QueueHandle_t xButtonQueue, xDisplayHandlerQueue;

//def in display handler
Base base;
Sprite sprites[]; //10 inst max
//
                  
void setup(){
    display_setup();
    button_setup();
}

//application code for now runs through here
void main_task(void *pvParameters) {

    //process button queue, and convert the user input into system commands.
    //ie; press up and sprite moves up
    //
    for( ;; ){
        printf("periodic main task\n");
        vTaskDelay(pdMS_TO_TICKS(5000));

        uint8_t recv;
        xQueueReceive(xButtonQueue, &recv, 0);
        if(recv) {
            //process here:
            if(recv==6){ //left??
                //FOR EXAMPLE
                spriteInfo msg;
                msg.sprite = &base.spriteArr[1];
                msg.x = (msg.sprite->x) - 1;
                msg.y = (msg.sprite->y);
                msg.tilemap = (msg.sprite->mapBuf);

                xQueueSendToBack(xDisplayHandlerQueue, &msg, 10);
            }
            else if(recv==7){
            }
            else if(recv==8){
            }
            else if(recv==9){ //these should be header defs
            }

        } 
        recv = 0;
        

    }
    
}

//to scaffold: 1) sdc general on-wire funcs spi (same bus)
//             2) sound .wav file buffering system

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
