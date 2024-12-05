//this file handles the en-masse 16-bit (half-word) transfer of system ram buffer's memory to peripheral
//(for now that's the display ram of the ili9341.)
//
//in the hopes that i can reach an fps that is suitable; around 60 fps
//all dma handling can be done using subroutines and a pointer to the buffer data, most likely.
//
#include "hardware/dma.h"

void dma_16_handler(spi_inst_t* spi_inst, uint16_t* dataBuf, size_t size); 

//error handling shizzle
