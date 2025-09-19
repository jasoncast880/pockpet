#ifndef PINOUT_H
#define PINOUT_H

//define the macros for all pins in an adjustable and visible single file
//ALL VARS IN THIS FILE REFER TO GPIO PINS

//USR INPUT
#define BTN_LEFT   6
#define BTN_UP     7
#define BTN_RIGHT  8
#define BTN_DOWN   9

#define BTN_A      2
#define BTN_B      3

#define BTN_START  4
#define BTN_SELECT 5

#define SPI0_TX    16
#define SPI0_RX    19
#define SPI0_SCLK  18

#define ILI9341_CS  17 //on spi0 bus
#define ILI9341_RST 20
#define ILI9341_DC  21

#define SDC_CS      22 //also on spi0 bus

//audio pins will define here

#endif //PINOUT_H
