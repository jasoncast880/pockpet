#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ST7735_TFT.hpp"

#include <string>

#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

ST7735_TFT myTFT;

//Debugging Text Animations
void BoolStatusUpdate();
void TCP_ClientUpdate();

void tftSetup(void) {
	stdio_init_all(); // Initialize chosen serial port
	TFT_MILLISEC_DELAY(1000);
	printf("TFT :: Start\r\n");

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = true; // true for hardware spi,

	if (bhardwareSPI == true) {								   // hw spi
		uint32_t TFT_SCLK_FREQ = 8000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else {								 // sw spi
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

  sleep_ms(500);
}

//globs
bool button_pressed = false;

void BoolStatusUpdate(){
  if(button_pressed){
    char teststr1[] = "Hello";
    char teststr2[] = "World";
    
    myTFT.TFTfillScreen(ST7735_BLACK);
    myTFT.TFTFontNum(myTFT.TFTFont_Default);
    myTFT.TFTdrawText(15, 15, teststr1, ST7735_WHITE, ST7735_BLACK, 2);
    myTFT.TFTdrawText(15, 35, teststr2, ST7735_WHITE, ST7735_BLACK, 2);
  }
  else {
    char teststr1[] = "Goodbye";
    char teststr2[] = "World";
    
    myTFT.TFTfillScreen(ST7735_BLACK);
    myTFT.TFTFontNum(myTFT.TFTFont_Default);
    myTFT.TFTdrawText(15, 15, teststr1, ST7735_WHITE, ST7735_BLACK, 2);
    myTFT.TFTdrawText(15, 35, teststr2, ST7735_WHITE, ST7735_BLACK, 2);
  }
}

void button_isr(uint gpio, uint32_t events) {
  button_pressed = !button_pressed;
}

int main() {
  tftSetup();

  //for now, 1 button setting a flag should be enough trouble to debug as is; once the desired results are achieved using callback functions i will integrate flags in order to scale for multiple buttons to handle a state.

  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);

  gpio_set_irq_enabled_with_callback(BUTTON1, GPIO_IRQ_EDGE_FALL, true, &button_isr);

  while(1) {
    BoolStatusUpdate();    
    sleep_ms(1500);
  }
}


