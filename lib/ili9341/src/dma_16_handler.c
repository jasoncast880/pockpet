#include "dma_16_handler.h"

void dma_16_handler(spi_inst_t* spi_inst, uint16_t* dataBuf, size_t length){
    
    int dma_chan = dma_claim_unused_channel(true); //claim an available DMA channel

    dma_channel_config c = dma_channel_get_default_config(dma_chan) 
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    channel_config_set_dreq(&c, DREQ_SPI0_TX);

    dma_channel_configure(
        dma_chan,
        &c,
        &spi_get_hw(spi0)->dr, //google 'rp 2040 register'
        dataBuf,
        length,
        false
    );
    
    
}


