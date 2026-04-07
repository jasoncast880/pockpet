#ifndef SDC_H
#define SDC_H

#include "pico/stdlib.h"
#include <cstddef>
#include <hardware/irq.h>
#include <hardware/spi.h>

#include "pinout.h"
#include "sdc_spi.h"
#include "ff.h" //make sur this gets included

#ifdef RTOS_MODE
#include "sync_common.h"

extern "C" {
void sdc_task( void* pvParameters );
}

#endif 


class SDHandler {

private:
	SDHandler();
	~SDHandler();

public:
	//shared/sync resources here
	
	static SDHandler& setup();
	SDHandler(const SDHandler& copy) = delete; 
	SDHandler& operator=(const SDHandler& copy) = delete; 

};

#endif //SDC_H
