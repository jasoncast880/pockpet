//purpose of this file: 
// 1) basic LCD configuration, setup display using the repo's ili9341 HAL library
// 2) interface LCD Handler's structs, unique data types <=> ili9341 HAL & functionalities in a thread-safe way.
// 3) expose debugging-relevant variables to a gdb-friendly interface

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include "FrameHandler.h"

#include "pinout.h"
#include "pico/stdlib.h"
#include "ili9341.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" { 
#endif

static SemaphoreHandle_t xDisplaySemaphore;
extern QueueHandle_t xDisplayHandlerQueue;

void lcd_render_task(void* pvParameters); 
void lcd_write_task(void* pvParameters); 

void driver_test();

#ifdef __cplusplus
}
#endif

void display_setup();

#endif //DISPLAYHANDLER_H
