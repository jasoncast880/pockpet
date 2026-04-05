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

static uint64_t sdc_cmd;
static uint8_t recv_buf[5];

static uint8_t generate_CRC_7(uint64_t val);
static uint16_t generate_CRC_16(uint64_t val); //optional, unnecessary

static int send_cmd(uint8_t idx, uint32_t arg);
static int send_data(uint32_t* buf, size_t size);

static int recv(uint8_t* buf, size_t size);



#endif //SDC_H
