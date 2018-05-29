

#ifndef _BUILTINS_H
#define _BUILTINS_H

#include "RGBConverter_t.h"

#define ITERATE_ALL_LEDS(Type,Num) for(Type Num=0;Num<LEDS.NumPixels();Num++)
#define ITERATE_FOR(Type,Num,Start,For) for(Type Num=Start;Num<(Start+For);Num++)
#define ITERATE_BETWEEN_INC(Type,Num,Start,End) for(Type Num=Start;Num<=End;Num++)
#define ITERATE_BETWEEN_EXC(Type,Num,Start,End) for(Type Num=Start;Num<End;Num++)

void SetSolidColour(RGBPixel Colour);

void SetSolidColour(HSLPixel Colour);

void SetSolidColour(HSVPixel Colour);

void SetSolidColour(u_int32_t Start, u_int32_t End, RGBPixel Colour);

void SetSolidColour(u_int32_t Start, u_int32_t End, HSLPixel Colour);

void SetSolidSolidColour(u_int32_t Start, u_int32_t End, HSVPixel Colour);

#endif //_BUILTINS_H
