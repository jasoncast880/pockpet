#include "sdc_spi.h"
#include <hardware/spi.h>

static spi_inst_t* _SDC_BUS;

static uint8_t _SDC_CS;
static uint8_t _SDC_SCLK;
static uint8_t _SDC_MOSI;
static uint8_t _SDC_MISO;

void sdc_initialize(int8_t cs, spi_inst_t* bus) {
}

static uint8_t generate_CRC_7(uint64_t val) {
	/*
	 * The CRC7 check is used for all commands, for all responses except 
	 * type R3, and for the CSD and CID registers.
	 * The CRC7 is a 7-bit value and is computed as follows:
	 * Generator polynomial: G(x) = x7 + x3 + 1.
	 * M(x) = (first bit) * xn + (second bit) * xn-1 +...+ (last bit) * x0
	 * CRC[6...0] = Remainder [(M(x) * x7)/G(x)]
	 */

	uint8_t crc = 0x00;

	for( int i = 0 ; i < 40 ; i++ ) {

		uint8_t bit = ( (uint8_t) ( (val >> (39-i)) & 1u ) ) ;

        uint8_t feedback = (uint8_t) ( (crc >> 6) & 1u ) ^ bit;
        crc = (crc << 1) & 0x7f;

        if(feedback) {
            crc ^= 0x09; // crc ^ 0b0000'1001
        }
	}
    return crc; //to use: discard first bit, use the rest
}

static int send_cmd(uint8_t idx, uint32_t arg) {
    uint64_t cmd = 0;

    cmd |= ( uint64_t )0 << 47; //'01' start-padding
    cmd |= ( uint64_t )1 << 46;

    cmd |= ( uint64_t )( idx & 0x3f ) << 40;

    cmd |= ( uint64_t ) arg << 8;

    uint8_t crc = generate_CRC_7(cmd>>8);
    cmd |= ( uint64_t )( crc & 0x7f ) << 1;

    cmd |= 1u; //end bit padding
    
    uint8_t bytes[6];
    bytes[0] = (uint8_t)(cmd>>40) & 0xff;
    bytes[1] = (uint8_t)(cmd>>32) & 0xff;
    bytes[2] = (uint8_t)(cmd>>24) & 0xff;
    bytes[3] = (uint8_t)(cmd>>16) & 0xff;
    bytes[4] = (uint8_t)(cmd>>8)  & 0xff;
    bytes[5] = (uint8_t)cmd       & 0xff;

    spi_write_blocking(_SDC_BUS, &bytes[0], 6);
    return 0;
}

static int recv(uint8_t* buf, size_t size) {
    for(int i = 0 ; i < size ; i++) {
        buf[i] = 0;
    }
    spi_read_blocking(_SDC_BUS, 0xff, buf, size);
    return 0 ;
}

int sdc_CS_LO() { gpio_put(_SDC_CS , 0); }
int sdc_CS_HI() { gpio_put(_SDC_CS , 1); }
