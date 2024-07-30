#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/rand.h"
#include "ST7735_TFT.hpp"
#include "ST7735_TFT_Assets.hpp"

#include <string>

#define POCKPET_CURTAIN 0xfcb6
#define POCKPET_WALL 0xff7e

#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

ST7735_TFT myTFT;

#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4

void ClearFloor();
void EatAnimation();
void WalkAnimation();
void SleepAnimation();

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

  //Render Background
  myTFT.TFTfillScreen(POCKPET_WALL);
  myTFT.TFTfillRectangle(0,0,128,36,POCKPET_CURTAIN);

  myTFT.TFTdrawBitmap16Data( 0, 36, (uint8_t *)pcurtain_bg, 128, 14);
  myTFT.TFTdrawBitmap16Data( 0, 112, (uint8_t *)pfloor_bg, 128, 47);
  sleep_ms(500);
}

float scale(float x, float a, float b, float min, float max)
{
  return (b-1)*(x-min)/(max-min)+a;
}

void ClearFloor()
{
  //clear the thingy
  myTFT.TFTdrawRectWH(0,50,128,61,POCKPET_WALL);
}


int xPos=0;
#define X_MAX_VAL = 65;

#define BUTTON1 2
#define BUTTON2 3
#define BUTTON3 4


bool sleepStatus = false;

void WalkAnimation()
{
  ClearFloor();

  while(1)
  {
    const uint8_t* spriteArrFwd[4] = {pwalking_fwd1,pwalking_fwd2,pwalking_fwd1,pwalking_fwd3};
    const uint8_t* spriteArrBck[4] = {pwalking_bck1,pwalking_bck2,pwalking_bck1,pwalking_bck3};
    
    int idx = 0;

    //go a random distance forward
    uint32_t r = get_rand_32();
    int dist = int(scale(r, 15, 65, 0, UINT32_MAX));
    printf("\n fwd %d", dist);
    for(int i=0; i<=dist; i++)
    {
      if(gpio_get(BUTTON1)==0)
      {
        printf("b1 pressed");
        EatAnimation();
      }
      else if(gpio_get(BUTTON2)==0)
      {
        printf("b2 pressed");
        SleepAnimation();
      }
      else if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed");
        ClearFloor();
      }
      myTFT.TFTdrawBitmap16Data(xPos,59,(uint8_t*)spriteArrFwd[idx],63,53);
      idx= idx >= 3 ? 0 : idx+1;
      xPos+=1;
      if(xPos >= 65)
      {
        //change dir to backwards
        break;
      }
      sleep_ms(350);
    }
      
    idx=0;

    //go a random distance backwards
    r=get_rand_32();
    dist=int(scale(r,15,65,0,UINT32_MAX));

    for(int i=0;i<=dist;i++)
    {
      if(gpio_get(BUTTON1)==0)
      {
        printf("b1 pressed");
        EatAnimation();
      }
      else if(gpio_get(BUTTON2)==0)
      {
        printf("b2 pressed");
        SleepAnimation();
      }
      else if(gpio_get(BUTTON3)==0)
      {
        printf("b3 pressed");
        ClearFloor();
      }
      myTFT.TFTdrawBitmap16Data(xPos,59,(uint8_t*)spriteArrBck[idx],63,53);
      idx= idx >= 3 ? 0 : idx+1;
      xPos-=1;
      if(xPos <= 10)
      {
        //change dir to forward 
        break;
      }
      sleep_ms(350);
    }
  }
}

void EatAnimation()
{
  ClearFloor();

  const uint8_t* dogArr[4] = {peating1, peating2, peating3, peating4};
  const uint8_t* steakArr[5] = {psteak1,psteak2,psteak3,psteak4,psteak5};

  int idx = 0;
  for(int i=0;i<5;i++)
  {
    myTFT.TFTdrawBitmap16Data(10,59,(uint8_t*)dogArr[idx],63,53);
    myTFT.TFTdrawBitmap16Data(80,59,(uint8_t*)steakArr[i],40,40);
    idx = idx >= 4 ? 0 : idx+1;

    //implement button handling here
    if(gpio_get(BUTTON1)==0)
    {
      //cancel eat animation.
    }
    else if(gpio_get(BUTTON2)==0)
    {
      //cancel eat animation.
    }
    else if(gpio_get(BUTTON3)==0)
    {
      WalkAnimation;
    }
    sleep_ms(500);
  }
  
  SleepAnimation();
}

void SleepAnimation()
{
  int idx=1;
  const uint8_t* dogArr[4] = {psleeping1, psleeping2, psleeping3, psleeping4};

  myTFT.TFTdrawBitmap16Data(xPos,59,(uint8_t*)dogArr[0],63,53);
  sleep_ms(1500);
  
  while(1)
  {
    if(gpio_get(BUTTON1)==0)
    {
      printf("b1 pressed");
    }
    else if(gpio_get(BUTTON2)==0)
    {
      printf("b2 pressed");
    }
    else if(gpio_get(BUTTON3)==0)
    {
      //go to walk animation
      printf("b3 pressed");
      myTFT.TFTdrawBitmap16Data(xPos,59,(uint8_t*)dogArr[0],63,53);
      sleep_ms(500); //change value later
      WalkAnimation();
    }

  //scroll through the sleeping frames, collect button data and run functions accordingly
    myTFT.TFTdrawBitmap16Data(xPos,59,(uint8_t*)dogArr[idx],63,53);
    idx = (idx == 4) ? 1 : idx+1;    
    sleep_ms(500);
  }
}


int main()
{
  tftSetup();

  gpio_init(BUTTON1);
  gpio_set_dir(BUTTON1, GPIO_IN);
  gpio_pull_up(BUTTON1);

  gpio_init(BUTTON2);
  gpio_set_dir(BUTTON2, GPIO_IN);
  gpio_pull_up(BUTTON2);

  gpio_init(BUTTON3);
  gpio_set_dir(BUTTON3, GPIO_IN);
  gpio_pull_up(BUTTON3);

  WalkAnimation();
}


