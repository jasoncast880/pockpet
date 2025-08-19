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

//need a queue to send from system to

extern Base base;
extern Sprite sprites[10];

extern SemaphoreHandle_t xDisplaySemaphore;

//global tasks; do in c for name manglin
#ifdef __cplusplus
extern "C" { 
#endif

void lcd_render_task(void* pvParameters); //
void lcd_write_task(void* pvParameters); //spi write to lcd

#ifdef __cplusplus
}                                         
#endif

//dedicated display handler SINGLETON class
class DisplayHandler { 
private:
    //make a singleton class
    DisplayHandler(); //assign frame memory map, if need be
    DisplayHandler(const DisplayHandler&) = delete;
    const DisplayHandler& operator=(const DisplayHandler&) = delete;

public:
    static DisplayHandler& GetInstance();

    void writeBuf(uint16_t* buf, size_t size);
    void writeByte(uint8_t byte);

    Tileset* getTileset();
};

#endif //DISPLAYHANDLER_H
