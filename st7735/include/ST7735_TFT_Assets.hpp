#pragma once

#include <cstdint>
#include "pico/stdlib.h"

extern const uint8_t * pfloor_bg; // 128x47 ; (0,113)
extern const uint8_t * pcurtain_bg; //128x14 ; (0,36) - (127,34)

//steaks!!!
extern const uint8_t * psteak1; //40x40 ; (x, 72)
extern const uint8_t * psteak2; 
extern const uint8_t * psteak3; 
extern const uint8_t * psteak4; 
extern const uint8_t * psteak5; 

//icons + alt icons ; TOP SIDE
extern const uint8_t * peat_icon; //29x29 ; (6,6)
extern const uint8_t * peat_icon_alt; 

extern const uint8_t * psleep_icon; // (50,6)
extern const uint8_t * psleep_icon_alt;

extern const uint8_t * pheart_icon; // (93,6)
extern const uint8_t * pheart_icon_alt; 

//button icons; BOT SIDE
extern const uint8_t * pleft_arrow; //26x26 ; (9,127)
extern const uint8_t * pleft_arrow_alt; 

extern const uint8_t * pright_arrow; // (51,127)
extern const uint8_t * psleep_icon_alt;

extern const uint8_t * pheart_icon; // (93,127)
extern const uint8_t * pheart_icon_alt; 

//facing right
extern const uint8_t * pwalking_fwd1; //63x53 ;
extern const uint8_t * pwalking_fwd2; 
extern const uint8_t * pwalking_fwd3; 

//facing left
extern const uint8_t * pwalking_bck1; //63x53 ;
extern const uint8_t * pwalking_bck2; 
extern const uint8_t * pwalking_bck3; 

//eating
extern const uint8_t * peating1; //63x53 ;
extern const uint8_t * peating2; 
extern const uint8_t * peating3;
extern const uint8_t * peating4;

//barking
extern const uint8_t * pbarking1; //63x53 ;
extern const uint8_t * pbarking2;
extern const uint8_t * pbarking3; 
extern const uint8_t * pbarking4;

//sleeping
extern const uint8_t * psleeping1; //63x53 ;
extern const uint8_t * psleeping2;
extern const uint8_t * psleeping3;
extern const uint8_t * psleeping4;

//congratulations
extern const uint8_t * pcongratulations1; //63x53
extern const uint8_t * pcongratulations2; 
extern const uint8_t * pcongratulations3; 
extern const uint8_t * pcongratulations4; 
extern const uint8_t * pcongratulations5; 
