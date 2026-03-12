#ifndef SYSTEM_H
#define SYSTEM_H
void 

static void font_setup(); //TODO insert the font tileset and hash.

//input
#include "buttons.h"
#include "usb.h"
inline static volatile bool use_usb = false;
void inputStreamTask( void * pvParameters );

//power
static volatile uint8_t batt_lvl;
void powerMonitorTask( void * pvParameters );
void lowBatteryTask( void * pvParameters );
void normalBatteryTask( void * pvParameters );


#endif // SYSTEM_H
