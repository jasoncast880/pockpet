// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Bitmap_Data.hpp"

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000
#define CLOCK_DISPLAY_TIME 20 // seconds
// push button
#define BUTTON1_GPIO 11
#define BUTTON2_GPIO 12
#define BUTTON3_GPIO 13

// Section :: Globals
ST7735_TFT myTFT;

//  Section ::  Function Headers

void Setup(void);	// setup + user options
void Test16BMP(void);
void WalkAnimation(void);
void WalkAnimationBG(void);
void ButtonTest(void);
//  Section ::  MAIN

int main(void)
{
	Setup();
//	Test16BMP();
  WalkAnimationBG();
  ButtonTest();

}
// *** End OF MAIN **

//  Section ::  Function Space

  /*!
	@brief  setup + user options
*/
void Setup(void)
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
	// the chosen interface (eg Spi0 CLK=18 DIN=19)
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


  //buttons setup
  gpio_init(BUTTON1_GPIO);
  gpio_set_dir(BUTTON1_GPIO, GPIO_IN);
  gpio_init(BUTTON2_GPIO);
  gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON1_GPIO);
  gpio_pull_up(BUTTON2_GPIO);
}

void Test16BMP(void)
{
	myTFT.TFTfillScreen(ST7735_BLACK);
  char teststr1[] = "Test 16 BMP";
	myTFT.TFTdrawText(5, 5, teststr1, ST7735_WHITE, ST7735_BLACK, 1);
	TFT_MILLISEC_DELAY(TEST_DELAY5);

	myTFT.TFTdrawBitmap16Data(0, 0, (uint8_t *)pOreo1, 64, 64);
	TFT_MILLISEC_DELAY(TEST_DELAY5);
	myTFT.TFTfillScreen(ST7735_BLACK);
}

void WalkAnimationBG(void) //how to make the background layer???
{
  static int y = 65;
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 160);
  TFT_MILLISEC_DELAY(TEST_DELAY2);

  for(uint8_t counter=0;counter<=85;counter+=4){
    myTFT.TFTdrawBitmap16Data(counter, y, (uint8_t *)pOreo1, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 160);

    myTFT.TFTdrawBitmap16Data(counter+1, y, (uint8_t *)pOreo2, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 160);

    myTFT.TFTdrawBitmap16Data(counter+2, y, (uint8_t *)pOreo1, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 160);

    myTFT.TFTdrawBitmap16Data(counter+3, y, (uint8_t *)pOreo3, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 160);
	  //TFT_MILLISEC_DELAY(TEST_DELAY1);
  }
  char teststrsus[]="Animation over";
	myTFT.TFTdrawText(5, 150, teststrsus, ST7735_WHITE, ST7735_BLACK, 1);
}
    
void WalkAnimation(void) //void for now, add pointer pathing to a list of bmp arrays or etc PLS make more elegant later
{
  static int y = 65;
  myTFT.TFTfillScreen(ST7735_WHITE);
  for(uint8_t counter=0;counter<=85;counter+=4){
    myTFT.TFTdrawBitmap16Data(counter, y, (uint8_t *)pOreo1, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTfillRectangle(counter, y, 1, 64, ST7735_WHITE);

    myTFT.TFTdrawBitmap16Data(counter+1, y, (uint8_t *)pOreo2, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTfillRectangle(counter+1, y, 1, 64, ST7735_WHITE);

    myTFT.TFTdrawBitmap16Data(counter+2, y, (uint8_t *)pOreo1, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTfillRectangle(counter+2, y, 1, 64, ST7735_WHITE);

    myTFT.TFTdrawBitmap16Data(counter+3, y, (uint8_t *)pOreo3, 64, 64);
	  TFT_MILLISEC_DELAY(100);
    myTFT.TFTfillRectangle(counter+3, y, 1, 64, ST7735_WHITE);
	  //TFT_MILLISEC_DELAY(TEST_DELAY1);
  }
  char teststrsus[]="Animation over";
	myTFT.TFTdrawText(5, 150, teststrsus, ST7735_WHITE, ST7735_BLACK, 1);
}

void ButtonTest(void)
{
  while(true) {
    if (!gpio_get(BUTTON1_GPIO)){
      myTFT.TFTfillScreen(ST7735_RED);
    }
    if (!gpio_get(BUTTON2_GPIO)){
      myTFT.TFTfillScreen(ST7735_BLUE);
    }
  TFT_MILLISEC_DELAY(100);
  }
}
