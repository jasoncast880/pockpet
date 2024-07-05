#pragma once

//Section: includes
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "ST7735_TFT_graphics.hpp"

// Section: Classes; Sprite class has a pricate field for pointers to the proper bmp arrays.
// destructor should be called to free dynamically allocated memory
//

/*!
* @brief Class to represent Sprite objects on different planes
 */
class Sprite{
  public:
    Sprite(int layer, const uint8_t * spriteArray, int arrSize); 
    void displayCurrentFrame(); 
    ~Sprite();

  private:
    int layer;
    const uint8_t * spriteArray; //dynamically allocated array
    int arrSize;
      
    //internal screen handling vars and methods
    int frameState;
    void clr();
};
