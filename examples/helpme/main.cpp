// Section ::  libraries
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Assets.hpp"
#include <string>
#include <iostream>

// Section :: Defines
//  Test timing related defines
#define TEST_DELAY1 1000
#define TEST_DELAY2 2000
#define TEST_DELAY5 5000

// push button
#define BUTTON1_GPIO 2
#define BUTTON2_GPIO 3
#define BUTTON3_GPIO 4

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
  gpio_init(BUTTON2_GPIO);
  gpio_set_dir(BUTTON2_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON2_GPIO);
}

class BaseComponent;

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

class AirplaneComponent : public BaseComponent {
public:
  void FlyForward() {
    std::cout << "AirplaneComponent is Flying Forward\n";
    myTFT.TFTfillScreen(0xa548);
    _controller->Notify(this, "Forward");
  }
  void Landing() {
    std::cout << "AirplaneComponent is Landing\n";
    myTFT.TFTfillScreen(0xffff);
    _controller->Notify(this, "Land");
  }
};


class WeatherComponent : public BaseComponent {
public:
  void Raining() {
    std::cout << "WeatherComponent begins raining\n";
    _controller->Notify(this, "Rain");
  }
  void Sunshine() {
    std::cout << "WeatherComponent begins sun\n";
    _controller->Notify(this, "Sun");
  }
};


class ConcreteController : public IController {
private:
  AirplaneComponent* airplanecomp;
  WeatherComponent* weathercomp;
public:
  ConcreteController(AirplaneComponent* ac,  WeatherComponent* wc) : airplanecomp(ac), weathercomp(wc) {
    airplanecomp->set_controller(this);
    weathercomp->set_controller(this);
  }

  void Notify(BaseComponent* sender, std::string event) const override { 
    std::cout << "event recieved: " << event << std::endl;
    if (event == "Sun") {
      airplanecomp->Landing();
    }
    if (event == "Land") {
      std::cout << "airplane is landing\n";
    }
  }
};


int main() {
  AirplaneComponent* airplane = new AirplaneComponent;
  WeatherComponent* weather = new WeatherComponent;

  ConcreteController* controller = new ConcreteController(airplane, weather);

  airplane->Landing();

  TFT_MILLISEC_DELAY(TEST_DELAY1);

  airplane->FlyForward();

  delete airplane;
  delete weather;
  delete controller;

  return 0;
}
