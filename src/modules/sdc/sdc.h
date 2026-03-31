#ifndef SDC_H
#define SDC_H

void sdc_setup();

#ifdef RTOS_MODE
extern "C" {
void sdc_task( void* pvParameters );
}
#endif 

#endif //SDC_H
