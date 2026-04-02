#ifndef SDC_H
#define SDC_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

//gp defines; 

//general-purpose registers
#define CMD0    0x00

void sdc_initialize(int8_t cs, spi_inst_t* bus); //running on spi0 bus
void sdc_writeCommand(uint8_t commandByte);
void sdc_writeDataBuffer8(uint8_t* dataBuf, size_t len);
void sdc_writeDataBuffer16(uint16_t* dataBuf, size_t len);

#endif //SDC_H
