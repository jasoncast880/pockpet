/*
 * ! displayHandler.h
 * @brief this handler exists to implement an async context that handles 
 * display tasks in an rtos-friendly manner.
 * due to the event-driven nature of display/hmi, i can use async_context rather than
 * tasks in order to save on tcb creation space.
 *
 * All input recieved from control signals (buttons) and from interrupts caused by 
 * other modules (wifi, sdc module) will send signals to the display Queue and 
 * be handled accordingly by the worker thread
 * !
 */

#ifndef _DISPLAY_HANDLER_H
#define _DISPLAY_HANDLER_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "ili9341.h" //for raw pixel handling 
#include "FrameHandler.h" //for tilebased and some bmp/asset file handling

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"
#include "task.h"

#include "pico/async_context_freertos.h"

#ifdef __cplusplus
extern "C"{
#endif


//kernel structures/tasks, async_contexts relevant
QueueHandle_t displayQueue; 
async_at_time_worker_t queue_worker;
static async_context_freertos_t async_ctx;

//create an async context
static async_context_t* get_async_ctx(void);

//use context to read queue, spawn appropriate async context to handle. consume queue
static void displayHandling(async_context_t* context, async_at_time_worker_t *worker);
//use 'worker_timeout' as the display's worker thread

static bool initDisplay(); //hardcode the pins in the method directly, spawn the callback asynchronousely

//message struct defs, sub defs
typedef enum{
    LCD_CMD_FILL_RECT,
    LCD_CMD_DRAW_IMAGE,
    LCD_CMD_WR_TEXT,
    LCD_CMD_RENDER_SPRITE
} lcd_cmd_t;

typedef struct{
    lcd_cmd_t command; //4 BYTES
    uint16_t x,y; // 4 BY
    union {
        struct {uint16_t w,h; uint16_t color;} rect_color; //8 BYTES (MAX)
        struct {uint16_t w,h; uint8_t* img_data;} rect_img;
        const char *text;
    };
} lcd_msg_t; //pass this by copy through queue; 16 B total?? (with padding)


#ifdef __cplusplus
}
#endif

#endif // EOF _DISPLAY_HANDLER_H
