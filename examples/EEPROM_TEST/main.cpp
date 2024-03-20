// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>

//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push button
#define BUTTON1_GPIO 13
#define BUTTON2_GPIO 12
#define BUTTON3_GPIO 11

ST7735_TFT myTFT;



//various menu animations
int main(void) 
{
	Setup();
  //implement state machines here
  
  //RenderBg();
  //ButtonHandler();
  int buttonPress = 0;
  //implement a second core to handle the button flashing
  RenderMenu(); //render static components
  //end of initialization core0 scheme
  
  //initialize the core1 processes and the data sharing mechanisms
  int32_t res;
/*
  queue_init(&call_queue, sizeof(queue_entry_t), 2);
  multicore_launch_core1(core1_entry);

  queue_entry_t entry = {testBlink};
  queue_add_blocking(&call_queue, &entry);
*/
  while(true)
  {
    if(!gpio_get(BUTTON1_GPIO)){
      updateButtons();
      //do gpio1 things
      buttonPress-=1;
      buttonPress = (buttonPress<=0) ? 2 : buttonPress;
      lightButtons(buttonPress);

      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON2_GPIO)){
      updateButtons();
      //do gpio2 things
      buttonPress+=1;
      //use the commented out code when u ready to add more buttons
      buttonPress = (buttonPress>2) ? 1 : buttonPress;
      //buttonPress = (buttonPress>4) ? 4 : buttonPress;
      lightButtons(buttonPress);
      
      sleep_ms(1200);
    }
    if(!gpio_get(BUTTON3_GPIO)){


      sleep_ms(1200);
      //break;
    }
  }
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
	// the chosen interface (eg Spi0 CLK=18 DIN=19)	int8_t SDIN_TFT = 19; int8_t SCLK_TFT = 18;
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
  gpio_pull_up(BUTTON1_GPIO);

  gpio_init(BUTTON2_GPIO);
  gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON2_GPIO);
  
  gpio_init(BUTTON3_GPIO);
  gpio_set_dir(BUTTON3_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON3_GPIO);
}

void RenderBg(void)
{
  myTFT.TFTfillScreen(ST7735_WHITE);
  myTFT.TFTdrawBitmap16Data( 0, 0, (uint8_t *)pbg_1, 128, 140);
  TFT_MILLISEC_DELAY(TEST_DELAY2);
  //WalkAnimation();
}

void RenderMenu(void)
{
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
