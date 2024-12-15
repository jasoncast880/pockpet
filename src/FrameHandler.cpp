#include "pico/stdlib.h"

/*
 * @brief: this class will handle all of the buffer rendering *before* the sprites are added. tilemaps & sets used to reduce wram size.
 *
 * control the frame buffer within wram, and you control the display itself.
 * */

uint8_t frameBuffer[(240)*(320)] = new uint8_t[];

class rectBuffer{
public:
    virtual void Render() = 0; //all classes must render a thingie

};
