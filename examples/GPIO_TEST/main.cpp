/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>

#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push buttons
#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

/* global vars, stored data*/
ST7735_TFT myTFT;
int myBars = 2;

/* setup fxn for TFT screen */
void tftSetup(void)
{
	stdio_init_all(); // Initialize chosen serial port
	TFT_MILLISEC_DELAY(TEST_DELAY1);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//**********************************************************

	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface (eg Spi0 CLK=18 DIN=19)	int8_t SDIN_TFT = 19; int8_t SCLK_TFT = 18;
  int8_t SDIN_TFT = 19;
  int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 17;
	int8_t CS_TFT = 21;
	int8_t RST_TFT = 16;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 2;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 1;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_Black); // pass enum,4 choices,see README
	//**********************************************************
}
//

/* GPIO BUTTON SETUP */
void buttonSetup()
{
  //buttons setup
  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);

  gpio_init(BUTTON2);
  gpio_set_dir(BUTTON2, GPIO_IN);
  gpio_pull_up(BUTTON2);
  
  gpio_init(BUTTON3);
  gpio_set_dir(BUTTON3, GPIO_IN);
  gpio_pull_up(BUTTON3);
}
//


/* Menu Screen TFT functions */
void ShowBars(int numBars);
void updateButtons() //draw over buttons to reset blinking effect
{
  myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button1,42,27);
  myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button1,42,27);
}

void RenderMenu(void)
{
  printf("rendering menu");
  myTFT.TFTfillScreen(0xa548);
  myTFT.TFTdrawBitmap16Data(1, 1, (uint8_t *)poreo_portrait, 81, 97);
  myTFT.TFTdrawBitmap16Data(17, 100, (uint8_t *)pguage_bar, 62, 14);
  myTFT.TFTdrawBitmap16Data(17, 116, (uint8_t *)pguage_bar, 62, 14);
  myTFT.TFTdrawBitmap16Data(2, 100, (uint8_t *)pheart_icon, 14, 13);
  myTFT.TFTdrawBitmap16Data(2, 116, (uint8_t *)ppaw_icon, 14, 14);
  myTFT.TFTdrawBitmap16Data(0, 133, (uint8_t *)pbottom_menu, 128, 27);
  ShowBars(myBars);
  updateButtons();
}
void ShowBars(int numBars)
{
  myTFT.TFTfillRect(22,102,52,10,0xd68b);
  for (int i = 0; i < numBars; i++) {
    int offset = i*13;
    myTFT.TFTdrawBitmap16Data(22+offset, 102, (uint8_t *)pbar1,13,10);
  }
}
void lightButtons(int n)
{
  if (n == 1) 
  {
    myTFT.TFTdrawBitmap16Data(84,12, (uint8_t *)phome_button2,42,27);
    //draw over 1
  }
  else if (n == 2)
  {
    myTFT.TFTdrawBitmap16Data(84,42, (uint8_t *)pfood_button2,42,27);
    //draw 2
  }
}
//

/* toma screen functions */
void RenderBg(void)
{
  myTFT.TFTfillScreen(ST7735_WHITE);
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 140);
  TFT_MILLISEC_DELAY(TEST_DELAY2);
  //WalkAnimation();
}
/* Toma Screen main function */

/* Menu Screen main function 
 * I need to refactor to use interrupts 
 * */ 

void MenuScreen() 
{

  //implement state machines here
  
  //RenderBg();
  //ButtonHandler();
  int buttonPress = 0;
  RenderMenu(); //render static components
  
  while(true)
  {
    if(!gpio_get(BUTTON1)){
      printf("button1 pressed");
      updateButtons();
      //do gpio1 things
      buttonPress-=1;
      buttonPress = (buttonPress<=0) ? 2 : buttonPress;
      lightButtons(buttonPress);

      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON2)){
      printf("button2 pressed");
      updateButtons();
      //do gpio2 things
      buttonPress+=1;
      //use the commented out code when u ready to add more buttons
      buttonPress = (buttonPress>2) ? 1 : buttonPress;
      //buttonPress = (buttonPress>4) ? 4 : buttonPress;
      lightButtons(buttonPress);
      
      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON3)){
      printf("button3 pressed");
      myTFT.TFTfillScreen(0xa548);
      sleep_ms(1200);
      //break;
    }
  }
}
//

int main()
{
	tftSetup();
  buttonSetup();
  //run the toma screen
  MenuScreen();
}

/*
static char event_str[128];

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    // Put the GPIO event(s) that just happened into event_str
    // so we can print it
    gpio_event_string(event_str, events);
    printf("GPIO %d %s\n", gpio, event_str);
}

int main() {
    stdio_init_all();

    printf("Hello GPIO IRQ\n");
    gpio_set_irq_enabled_with_callback(2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Wait forever
    while (1);
}


static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}
*/
