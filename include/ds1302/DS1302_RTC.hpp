#pragma once;
#include "pico/stdlib.h"

//DS1302 Register Addresses
#define DS1302_REG_TIMEDATE = 0x80;
#define DS1302_REG_TIMEDATE_BURST = 0xBE;
#define DS1302_REG_TCR = 0x90;
#define DS1302_REG_RAM_BURST = 0xFE;
#define DS1302_REG_RAMSTART = 0xc0;
#define DS1302_REG_RAMEND = 0xfd;

//define 3wire command r/w flag
#define THREEWIRE_READFLAG = 0x01;
// ram read and write addresses are interleaved
const uint8_t DS1302RamSize = 31;

//define register commands

//gpio abstractions

class Serial3Wire
{
private:
  uint ce_pin_;
  uint data_pin_;
  uint clock_pin_;

public:
  Serial3Wire(uint ce_pin, uint data_pin, uint clock_pin) : /*pins here */;
  void sendByte(uint8_t byte);
  void setMode(uint8_t mode);

};

class DS1302_RTC : public Serial3Wire
{

public:
  DS1302_RTC();

  void set24H_Time();
  uint32_t get24H_Time();
  
private:
  void getReg(uint8_t);
  void setReg(uint8_t regAddr, uint8_t regValue);
};

//EOF
