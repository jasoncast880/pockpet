#ifndef GRAPHICS_CONF_H
#define GRAPHICS_CONF_H

//TODO: this file is to be used t configure common macros between the tile engine and the system's display module code. 
//IE the ram/rendered pixel buffer size should be configured here in order to tune the system acording to optimal rato between dma/hardware push speed and blitting/flash-to-ram render speed.

#define DEFAULT_TILE_LEN 16
#define DEFAULT_SCREEN_TILES_Y 15
#define DEFAULT_SCREEN_TILES_X 20
#define MAX_LAYERS 2
#define MAX_SPRITES_PER_LAYER 5


#endif //GRAPHICS_CONF_H
