#ifndef USB_H
#define USB_H

#include "class/cdc/cdc_device.h"
#include <stdlib.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include <pico/stdio.h>

#ifdef __cplusplus
extern "C" {
#endif 

void usb_setup();

//weak-linked from descriptors
void tud_cdc_rx_cb(uint8_t itf);
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts);

#include <FreeRTOS.h>
#include "task.h"
void usb_task( void* pvParameters ); 
void display_override_task( void* pvParameters ); 

#ifdef __cplusplus
}
#endif 

#endif //USB_H
