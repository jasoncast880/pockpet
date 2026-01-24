#ifndef USB_H
#define USB_H

#include <stdlib.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include <pico/stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#ifdef __cplusplus
extern "C" {
#endif 

void usb_send(void* pvParameters);
void usb_recv(void* pvParameters);

void usb01_irqh(void);
/**/
void usb_setup();

#ifdef __cplusplus
}
#endif 

#endif //USB_H
