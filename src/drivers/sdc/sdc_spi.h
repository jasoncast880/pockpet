#ifndef SDC_H
#define SDC_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

//COMMAND Defines ; max 6 bits width
#define CMD_GO_IDLE_ST  0x00
#define CMD8						0x08
#define CMD58   

//APP_COMMAND Defines: Note must be preceded with APP_CMD (CMD55)
#define ACMD41  

static uint32_t cmd [2]; //global accessible thing
static uint8_t generate_CRC_7(uint32_t* val);
static uint16_t generate_CRC_16(uint32_t* val);

//command is structured as 1 and a half word; thus to send a cmd you need to do a formatted buffer, based on cmd index, (optional) argument, crc, and app. padding as needed...
//perhaps 
int send_cmd(uint8_t idx, uint32_t arg);
int send_data(uint32_t* buf, size_t size);
int recv(uint32_t* buf, size_t size);

#endif //SDC_H
