#include "BuiltIns.h"
#include "Effects.h"
#include "lib8tion.h"

void SetSolidColour(RGBPixel Colour) {
    ITERATE_ALL_LEDS(u_int32_t, i) {
        LEDS.SetPixel(i, Colour);
    }
}

void SetSolidColour(HSLPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour, ThisCol);
    SetSolidColour(ThisCol);

}

void SetSolidColour(HSVPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour, ThisCol);
    SetSolidColour(ThisCol);
}

void SetSolidColour(u_int32_t Start, u_int32_t End, RGBPixel Colour) {
    ITERATE_BETWEEN_INC(u_int32_t, i, Start, End) {
        LEDS.SetPixel(i, Colour);
    }

}

void SetSolidColour(u_int32_t Start, u_int32_t End, HSLPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour, ThisCol);
    SetSolidColour(Start, End, ThisCol);
}

void SetSolidColour(u_int32_t Start, u_int32_t End, HSVPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour, ThisCol);
    SetSolidColour(Start, End, ThisCol);
}

void FillRainbow(u_int32_t FirstLED_ID, u_int32_t NumToFill,
                 float InitialHue,
                 float DeltaHue) {
    HSVPixel HSV;
    HSV.H = InitialHue;
    HSV.V = 1.0;
    HSV.S = 0.8;
    for (int i = 0; i < NumToFill; i++) {
        LEDS.SetPixel(FirstLED_ID + i, HSV);
        HSV.H += DeltaHue;
    }
}


void FillGradientRGB(uint16_t StartPos, RGBPixel StartColor, uint16_t EndPos, RGBPixel EndColor) {
    // if the points are in the wrong order, straighten them
    if (EndPos < StartPos) {
        uint16_t t = EndPos;
        RGBPixel tc = EndColor;
        EndColor = StartColor;
        EndPos = StartPos;
        StartPos = t;
        StartColor = tc;
    }

    u_int16_t RDistance;
    u_int16_t GDistance;
    u_int16_t BDistance;

    RDistance = (EndColor.R - StartColor.R) << 7;
    GDistance = (EndColor.G - StartColor.G) << 7;
    BDistance = (EndColor.B - StartColor.B) << 7;

    uint16_t PixelDistance = EndPos - StartPos;
    int16_t Divisor = PixelDistance ? PixelDistance : 1;

    u_int16_t RDelta = RDistance / Divisor;
    u_int16_t GDelta = GDistance / Divisor;
    u_int16_t BDelta = BDistance / Divisor;

    RDelta *= 2;
    GDelta *= 2;
    BDelta *= 2;

    u_int16_t R = StartColor.R << 8;
    u_int16_t G = StartColor.G << 8;
    u_int16_t B = StartColor.B << 8;
    for (uint16_t i = StartPos; i <= EndPos; i++) {
        LEDS.SetPixel(i, R >> 8, G >> 8, B >> 8);
        R += RDelta;
        G += GDelta;
        B += BDelta;
    }
}


void FillGradientRGB( uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2) {
    uint16_t Last = NumToFill - 1;
    FillGradientRGB(0, c1, Last, c2);
}


void FillGradientRGB(uint16_t NumToFill, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3) {
    uint16_t Half = (NumToFill / 2);
    uint16_t Last = NumToFill - 1;
    FillGradientRGB(0, c1, Half, c2);
    FillGradientRGB(Half, c2, Last, c3);
}

void FillGradientRGB(uint16_t NumLeds, const RGBPixel &c1, const RGBPixel &c2, const RGBPixel &c3, const RGBPixel &c4) {
    uint16_t OneThird = (NumLeds / 3);
    uint16_t TwoThirds = ((NumLeds * 2) / 3);
    uint16_t Last = NumLeds - 1;
    FillGradientRGB(0, c1, OneThird, c2);
    FillGradientRGB(OneThird, c2, TwoThirds, c3);
    FillGradientRGB(TwoThirds, c3, Last, c4);
}


void Scale(u_int32_t StartID, u_int32_t EndID, float Scale) {
    for (u_int32_t i = 0; i < EndID; i++) {
        LEDS.ScalePixel(i,Scale);
    }
}

void Fade(u_int32_t StartID, u_int32_t EndID, float FadeBy) {
    Scale(StartID, EndID, 1 - FadeBy);

}

void PrintPixel(u_int8_t R,u_int8_t G,u_int8_t B){
    Serial.print("Pixel - R:");
    Serial.print(R);
    Serial.print(" G:");
    Serial.print(G);
    Serial.print(" B:");
    Serial.println(B);
}

void PrintPixel(RGBPixel Colour){
    Serial.print("RGB");
    PrintPixel(Colour.R,Colour.G,Colour.B);
}

void PrintPixel(u_int32_t Colour){
    PrintPixel((Colour>>16 &255),(Colour>>8)&255,Colour&255);
}

void PrintPixel(HSLPixel Colour){
    Serial.print("HSLPixel - H:");
    Serial.print(Colour.H);
    Serial.print(" S:");
    Serial.print(Colour.S);
    Serial.print(" L:");
    Serial.print(Colour.L);
    Serial.print(" ");
    RGBPixel Temp;
    ColourConverter.ToRGB(Colour,Temp);
    PrintPixel(Temp);
}
void PrintPixel(HSVPixel Colour){
    Serial.print("HSVPixel - H:");
    Serial.print(Colour.H);
    Serial.print(" S:");
    Serial.print(Colour.S);
    Serial.print(" V:");
    Serial.print(Colour.V);
    Serial.print(" ");
    RGBPixel Temp;
    ColourConverter.ToRGB(Colour,Temp);
    PrintPixel(Temp);
}


