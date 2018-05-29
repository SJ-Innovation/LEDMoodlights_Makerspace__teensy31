

#ifndef _BUILTINS_H
#define _BUILTINS_H

#include "RGBConverter_t.h"
#include "Colours.h"
#define ITERATE_ALL_LEDS(Type, Num) for(Type Num=0;Num<LEDS.NumPixels();Num++)
#define ITERATE_FOR(Type, Num, Start, For) for(Type Num=Start;Num<(Start+For);Num++)
#define ITERATE_BETWEEN_INC(Type, Num, Start, End) for(Type Num=Start;Num<=End;Num++)
#define ITERATE_BETWEEN_EXC(Type, Num, Start, End) for(Type Num=Start;Num<End;Num++)

void SetSolidColour(RGBPixel Colour);

void SetSolidColour(HSLPixel Colour);

void SetSolidColour(HSVPixel Colour);

void SetSolidColour(u_int32_t Start, u_int32_t End, RGBPixel Colour);

void SetSolidColour(u_int32_t Start, u_int32_t End, HSLPixel Colour);

void SetSolidSolidColour(u_int32_t Start, u_int32_t End, HSVPixel Colour);

void FillRainbow(int FirstLED_ID, int NumToFill, uint8_t InitialHue, uint8_t DeltaHue);

void FillGradientRGB(uint16_t StartPos, RGBPixel StartColor, uint16_t EndPos, RGBPixel EndColor);

void FillGradientRGB(uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2);

void FillGradientRGB(uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3);

void FillGradientRGB(uint16_t NumLeds, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3, const RGBPixel &c4);

void Scale(u_int32_t StartID, u_int32_t EndID, float Scale);

void Fade(u_int32_t StartID, u_int32_t EndID, float FadeBy);

#endif //_BUILTINS_H
