#include "displayHandler.h"

// Priorities of our threads - higher numbers are higher priority
#define DISPLAY_WORKER_TASK_PRIORITY    ( tskIDLE_PRIORITY + 4UL )

// Stack sizes of our threads in words (4 bytes)
#define DISPLAY_WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

async_context_t* get_async_ctx(void){
    async_context_freertos_config_t config = async_context_freertos_default_config();
    config.task_priority = DISPLAY_WORKER_TASK_PRIORITY;
    config.task_stack_size = DISPLAY_WORKER_TASK_STACK_SIZE;

    if(!async_context_freertos_init(&async_ctx, &config)) return NULL;
        return &async_ctx.core;
}

void displayHandling(async_context_t* context, async_at_time_worker_t *worker){
    //check the queue after 10 ms !!!!
    async_context_add_at_time_worker_in_ms(context, worker,10); //call this method in 10 ms

    lcd_msg_t msg;
    while(xQueueReceive(displayQueue_h, &msg, 0) == pdPASS){
        if(msg.command==0){
            //fill rect w color
            ili9341_setAddrWindow(msg.x,msg.y,msg.rect_color.w,msg.rect_color.h);
            ili9341_writeCommand(RAM_WR);
            uint8_t color[2];
            color[0] = msg.rect_color.color >> 8;
            color[1] = msg.rect_color.color & 0xFF;
            for(int i = 0; i<(msg.rect_color.w*msg.rect_color.h);i++){
                ili9341_writeData(color[0]);
                ili9341_writeData(color[1]);
            }
        }
    }
}
async_at_time_worker_t worker_timeout = {.do_work = displayHandling}; // !!!!!!!

bool initDisplay(void){ // 'main' display task : call this from main as its own async??
//initialize spi (you can do pin assignments here) and run an introductory animation 
    ili9341_initialize(17,20,21,19,18,16); //pcb ver should use different pins

    //create the queue
    QueueHandle_t displayQueue_h = xQueueCreate(30,16); //30 items, 2 bytes

    //calls the queue check every 10 ms
    async_context_t *context = get_async_ctx();
    async_context_add_at_time_worker_in_ms(context, &worker_timeout,0);

    
    //add any things required in startup: animation, loading bar, etc..

    return true;
}

