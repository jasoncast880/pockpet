#ifndef SDC_H
#define SDC_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

//COMMAND Defines ; max 6 bits width
#define CMD_GO_IDLE_ST  0x00 //cmd00
#define SEND_IF_COND    0x08 //cmd08
#define READ_OCR        0x3A //cmd58

//APP_COMMAND Defines: Note must be preceded with APP_CMD (CMD55)
#define APP_CMD         0x37 //cmd55
#define SD_SEND_OP_COND 0x29 //acmd41

static uint64_t sdc_cmd;
static uint8_t recv_buf[5];

static uint8_t generate_CRC_7(uint64_t val);
static uint16_t generate_CRC_16(uint64_t val); //optional, unnecessary

static int send_cmd(uint8_t idx, uint32_t arg);
static int send_data(uint32_t* buf, size_t size);

static int recv(uint8_t* buf, size_t size);

int sdc_initialize(uint8_t cs, spi_inst_t* spi);

int sdc_CS_LO();
int sdc_CS_HI();

#endif //SDC_H
