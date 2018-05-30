

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

void FillRainbow(u_int32_t FirstLED_ID, u_int32_t NumToFill, float InitialHue, float DeltaHue);

void FillGradientRGB(uint16_t StartPos, RGBPixel StartColor, uint16_t EndPos, RGBPixel EndColor);

void FillGradientRGB(uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2);

void FillGradientRGB(uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3);

void FillGradientRGB(uint16_t NumLeds, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3, const RGBPixel &c4);

void Scale(u_int32_t StartID, u_int32_t EndID, float Scale);

void Fade(u_int32_t StartID, u_int32_t EndID, float FadeBy);
void PrintPixel(u_int8_t R,u_int8_t G,u_int8_t B);
void PrintPixel(RGBPixel Colour);
void PrintPixel(u_int32_t Colour);
void PrintPixel(HSLPixel Colour);
void PrintPixel(HSVPixel Colour);


#endif //_BUILTINS_H
