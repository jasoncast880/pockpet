// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push button
#define BUTTON1_GPIO 11
#define BUTTON2_GPIO 12
#define BUTTON3_GPIO 13

ST7735_TFT myTFT;

//do menu things

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
  gpio_init(BUTTON3_GPIO);
  gpio_set_dir(BUTTON3_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON1_GPIO);
  gpio_pull_up(BUTTON2_GPIO);
  gpio_pull_up(BUTTON3_GPIO);
}



class BaseComponent;

//vars
int bars = 0;

class IController {
public:
  virtual void Notify(BaseComponent* sender, std::string event) const = 0;
};

class BaseComponent {
protected:
  IController* _controller;
public: 
  BaseComponent(IController* controller = nullptr) : _controller(controller) {}
  void set_controller(IController* controller) {
    _controller = controller;
  }
};

class Button : public BaseComponent {
public:
  int x = 84;
  int y = 12;
  void Display() {
    _controller->Notify(this, "display");
  }
  void Blink() {
    _controller->Notify(this, "blink");
  }
};

class Gauge : public BaseComponent {
public: 
  void Display() {
    myTFT.TFTdrawBitmap16Data(17,100, (uint8_t *)pguage_bar,62,14); //ik the ptr speling is wrong....
    _controller -> Notify(this, "display");
  }
  void Add() {
    /*
    //add some data structure stuff here 
    myTFT.TFTdrawBitmap16Data(x,y, (uint8_t *)phome_button2,42,27);
    _controller -> Notify(this, "add");
    */
  }
  void Remove() {
  }
};
  
class ConcreteController : public IController {
private:
  Button* buttonComp;
  Gauge* gaugeComp;
public:
  ConcreteController( Button* bc, Gauge* gc) : buttonComp(bc), gaugeComp(gc) {
    buttonComp->set_controller(this);
    gaugeComp->set_controller(this);
  }
  
  void Notify(BaseComponent* sender, std::string event) const override {
    if (event == "display") {
      buttonComp->Display();
      myTFT.TFTdrawBitmap16Data(x,y, (uint8_t *)phome_button1,42,27);
    }
    if (event == "blink") {
      buttonComp->Blink();
      myTFT.TFTdrawBitmap16Data(x,y, (uint8_t *)phome_button2,42,27);
    }
  }
};
    
int main(void)
{
	Setup();
  myTFT.TFTfillScreen(0xa548);
  myTFT.TFTdrawBitmap16Data(1,1, (uint8_t *)poreo_portrait,81,97); 
  myTFT.TFTdrawBitmap16Data(0,133, (uint8_t *)pbottom_menu,128,27); 

  Button* homeButt = new Button; 
  Gauge* gauge = new Gauge; 

  ConcreteController* controller = new ConcreteController(homeButt, gauge);
    
  //bool lightState = true;
  homeButt->Display();
  TFT_MILLISEC_DELAY(TEST_DELAY2);
  myTFT.TFTfillScreen(0xa548);
  homeButt->Blink();

  delete homeButt;
  delete gauge;
  delete controller;
  return 0;
}
// *** End OF MAIN **

