#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "audio_assets.hpp"

// Class to control Speaker Module's basic functionalities
class Speaker_Driver
{
  public:
    Speaker_Driver();
    ~Speaker_Driver(){};

    void SetupPWM(int8_t);
    void Speak(uint32_t*); //run audio file w ptr param to the file in audio_assets.hpp
}
