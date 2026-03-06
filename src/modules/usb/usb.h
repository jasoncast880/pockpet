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

void usb_task();

//weak-linked from descriptors
void tud_cdc_tx_complete_cb(uint8_t itf); 
void tud_cdc_rx_complete_cb(uint8_t itf);
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts);

#ifdef __cplusplus
}
#endif 

#endif //USB_H
