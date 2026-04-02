#include "sdc_spi.h"
#include "pinout.h" //remember: using spi bus 0 for shared bus access
#include <hardware/spi.h>

static spi_inst_t* spi_bus;

static uint8_t _SDC_CS;
static uint8_t _SDC_SCLK;
static uint8_t _SDC_MOSI;
static uint8_t _SDC_MISO;

void sdc_initialize(int8_t cs, spi_inst_t* bus) {
	spi_bus = bus;

	_SDC_CS = cs;
	gpio_init(_SDC_CS);
	gpio_set_dir(_SDC_CS, GPIO_OUT);
	
	gpio_put(_SDC_CS, 1);

	uint8_t dummy = 0xFF;
	for(int i = 0 ; i<100 ; i++) {
		spi_write_blocking(spi_bus, &dummy, 1);
	}

	gpio_put(_SDC_CS, 0);
	dummy = CMD0;
	spi_write_blocking(spi_bus, &dummy, 1); //perhaps do an assert? idk
}

void sdc_writeCommand(uint8_t byte) {
}

void sdc_writeDataBuffer8(uint8_t* buf, size_t len) {
	spi_write_blocking(spi_bus, buf, len);
}

void sdc_writeDataBuffer16(uint16_t *buf, size_t len) {
	spi_write16_blocking(spi_bus, buf, len);
}
