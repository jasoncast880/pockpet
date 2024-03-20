#include "pico/stdlib.h"
#include "../include/ds1302/DS1302_RTC.hpp"

class Serial3Wire
//written for use with ds1302; will be on read mode by default. On write operations, change ce to 1, then data dir to GPIO_OUT, then once finished transmission change data dir to GPIO_IN, then CE back to 0.
{
private:
  uint ce_pin_;
  uint data_pin_;
  uint clock_pin_;

public:
  Serial3Wire(uint ce_pin, uint data_pin, uint clock_pin) : 
  ce_pin_(ce_pin), data_pin_(data_pin), clock_pin_(clock_pin)
  {
    gpio_init(ce_pin_);
    gpio_init(data_pin_);
    gpio_init(clock_pin_);
    gpio_set_dir(ce_pin_, GPIO_OUT);
    gpio_set_dir(data_pin_, GPIO_IN); //should always be read by default
    gpio_set_dir(clock_pin_, GPIO_OUT);
  }

  void readByte()
  {
  gpio_put(ce_pin_, 1);
    
    for(int i = 0; i<8; i++)
    {
      byte|=(gpio_get(data_pin_) << i);
      gpio_put(clock_pin_, 1);
      sleep_us(10);
      gpio_put(clock_pin_, 0);
      sleep_us(10);
    }
  gpio_put(ce_pin_, 0);
  }

  void writeByte(uint8_t byte) ///primarily to write command byte to the rtc reg.
  {
  gpio_put(ce_pin_, 1);
  //temp set value of data to out
  gpio_set_dir(data_pin_, GPIO_OUT);
    
    for(int i = 0; i<8; i++)
    {
      gpio_put(data_pin_, (byte>>i)&1); //send each bit from msb to lsb
      gpio_put(clock_pin_, 1);
      sleep_us(10);
      gpio_put(clock_pin_, 0);
      sleep_us(10);
    }

  //set data back to read
  gpio_set_dir(data_pin_, GPIO_IN);
  gpio_put(ce_pin_, 0);
  }
}

class PeripheralDriver : public Serial3Wire{
public:
  PeripheralDriver(uint ce_pin, uint data_pin, uint clock_pin) :
  Serial3Wire(ce_pin, data_pin, clock_pin);
 
  void burstModeWriteTimeDate(uint8_t[] byte)
  {
    writeByte(0xBE); // 1011 1110  write to clock @ address 31
    for(int i = 0; i<8; i++)
    {
      //honestly for now just implement game logic to write the time, no date
      //ALSO NOTE:
      //Im pretty sure that addressing will be inferred from the bytes i pass? not 100%
      writeByte(byte[i]);
    }
  }

  uint8_t burstModeReadTimeDate()
  {
    //initializers
    uint8_t startAddr() = 0x81;
    uint8_t regData[] = new uint8_t[8];

    writeByte(0xBF);
    for(int i = 0; i<8; i++)
    { 
      regData[i] = readByte(0x81+(2*i));
    }
    return regData;
  }

  void burstModeWriteTimeDate(uint8_t[] regData)
  {
    uint8_t startAddr() = 0x80;
    
    writeByte(0xBE);
    for(int i = 0; i<8; i++)
    {
      writeByte(regData[i]);
    }
  }

}
    
