// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include "pico/util/queue.h"
#include "pico/multicore.h"
#include <string>


// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push button
#define BUTTON2_GPIO 12

ST7735_TFT *myTFT;
mutex_t peripheral_mutex;

void core1_entry() 
{
  while(1)
  {
    mutex_enter_blocking(&peripheral_mutex);
    myTFT->sendData();
    mutex_exit(&peripheral_mutex);
    sleep_ms(1000);
    myTFT->TFTfillRect(1,1,30,30,0x0000);
    sleep_ms(1000);
    myTFT->TFTfillRect(1,1,30,30,0xffff);
    sleep_ms(1000);
  }
}


int myBars = 0;

void Setup(void);	// setup + user 
// RTC handling and related interrupts

// open backgrounds
void RenderMenu(void);

//animation for second core to run
void BlinkTest(void);
void ShowBars(int n);

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
		myTFT->TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT->TFTInitSPIType(SWSPICommDelay);
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
	myTFT->TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 2;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 1;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT->TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT->TFTInitPCBType(myTFT->TFT_ST7735S_Black); // pass enum,4 choices,see README
												 //**********************************************************

  //buttons setup
  //gpio_init(BUTTON1_GPIO);
  //gpio_set_dir(BUTTON1_GPIO, GPIO_IN);
  //gpio_pull_up(BUTTON1_GPIO);
  
  gpio_init(BUTTON2_GPIO);
  gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON2_GPIO);

  //gpio_init(BUTTON3_GPIO);
  //gpio_set_dir(BUTTON3_GPIO, GPIO_IN);
  //gpio_pull_up(BUTTON3_GPIO);
}

void BlinkTest(void) //run this on the second core??
{

  while(1)
  {
    myTFT->TFTfillRect(1,1,30,30,0x0000);
    sleep_ms(1000);
    myTFT->TFTfillRect(1,1,30,30,0xffff);
    sleep_ms(1000);
  }
}

void RenderMenu(void)
{ 
  //load all static components
  myTFT->TFTfillScreen(0xa548);
  myTFT->TFTdrawBitmap16Data(1, 1, (uint8_t *)poreo_portrait, 81, 97);
  myTFT->TFTdrawBitmap16Data(17, 100, (uint8_t *)pguage_bar, 62, 14);
  myTFT->TFTdrawBitmap16Data(17, 116, (uint8_t *)pguage_bar, 62, 14);
  myTFT->TFTdrawBitmap16Data(2, 100, (uint8_t *)pheart_icon, 14, 13);
  myTFT->TFTdrawBitmap16Data(2, 116, (uint8_t *)ppaw_icon, 14, 14);
  myTFT->TFTdrawBitmap16Data(0, 133, (uint8_t *)pbottom_menu, 128, 27);
  ShowBars(myBars);
  
  while(true)
  {
    if(!gpio_get(BUTTON2_GPIO)){
      
      //default increment expression 
      myBars+=1;
      myBars = (myBars>4) ? 4 : myBars;

      ShowBars(myBars);
      BlinkTest();
      sleep_ms(1500);
    }
  }
}
void ShowBars(int numBars)
{
  myTFT->TFTfillRect(22,102,52,10,0xd68b);
  for (int i = 0; i < numBars; i++) {
    int offset = i*13;
    myTFT->TFTdrawBitmap16Data(22+offset, 102, (uint8_t *)pbar1,13,10);
  }
}
 
int main(void)
{
  stdio_init_all();
	Setup();
  RenderMenu();


  // Initialize mutex
  mutex_init(&peripheral_mutex);

  // Start second core
  multicore_launch_core1(core1_entry);

  while(true) 
  {
    sleep_ms(1000);
  }

  return 0;
}


