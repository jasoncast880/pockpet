//purpose of this file: 
// 1) basic LCD configuration, setup display using the repo's ili9341 HAL library
// 2) interface LCD Handler's structs, unique data types <=> ili9341 HAL & functionalities in a thread-safe way.
// 3) expose debugging-relevant variables to a gdb-friendly interface

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "FrameHandler.h"
#include "pico/stdlib.h"
#include "ili9341.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct spriteInfo {
   Sprite* sprite;   //4 bytes
   uint16_t x,y;     //4 byte (x2 16-bit ints by value)
   uint8_t* tilemap; //4 bytes
}; //12 BY total

//must be global; used between the 2 
extern Base base;
extern Sprite sprites[10];

#ifdef __cplusplus
extern "C" { 
#endif

static SemaphoreHandle_t xDisplaySemaphore;
extern QueueHandle_t xDisplayHandlerQueue;

void lcd_render_task(void* pvParameters); 
void lcd_write_task(void* pvParameters); 

#ifdef __cplusplus
}
#endif

void display_setup();

#endif //DISPLAYHANDLER_H
