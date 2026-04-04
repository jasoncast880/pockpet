#include "sdc_spi.h"
#include <hardware/spi.h>

static spi_inst_t* _SDC_BUS;

static uint8_t _SDC_CS;
static uint8_t _SDC_SCLK;
static uint8_t _SDC_MOSI;
static uint8_t _SDC_MISO;

void sdc_initialize(int8_t cs, spi_inst_t* bus) {
}

static uint8_t generate_CRC_7(uint32_t* val) {
	/*
	 * The CRC7 check is used for all commands, for all responses except 
	 * type R3, and for the CSD and CID registers.
	 * The CRC7 is a 7-bit value and is computed as follows:
	 * Generator polynomial: G(x) = x7 + x3 + 1.
	 * M(x) = (first bit) * xn + (second bit) * xn-1 +...+ (last bit) * x0
	 * CRC[6...0] = Remainder [(M(x) * x7)/G(x)]
	 */
	uint8_t crc = 0x00;
	//bool		ans;
	//NOTE: using the last bit of the sequence, compare to the other characters to gen.
	//checksum

	for( int i = 0 ; i < 40 ; i++ ) {

		crc = ( (uint8_t)( ( (*val >> (40-i)) & 1u) << 8 ) ) & (crc >> 1) ;
		
		//get the values of index x0, x3 and xor accordingly, also considering branching
		uint8_t x0 = ( ( crc >> 7 ) & 1u ) ^ ( crc & 1u ) << 7;
		uint8_t x3 = ( ( crc >> 4 ) & 1u ) ^ ( crc & 1u ) << 4;
		
		crc = crc & x0 & x3; //to use this discard the right-most bit.
	}
}

int send_cmd(uint8_t idx, uint32_t arg) {
	//note global static array of 2 elements 'uint_32t cmd[2]'
	//cmd[0]: 2 bit padding '01' | index of cmd (6 bits) | first 24 bits of arg
	//cmd[1]: last 8 bits of arg | 7 bits of crc | '1' bit | Dont.Care last 16 bits.
	//send this string as 3 16 bit half-words, and expect 3 to be returned via sdk func.
	//seperate function does any await for response.
}

//responses are msb first
//0
int recv(uint8_t* buf) {
	spi_read_blocking(_SDC_BUS, 0, buf, 1);
	if(*buf) {
		return 1;
	} else return 0;
}
