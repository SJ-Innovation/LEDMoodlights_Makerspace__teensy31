#include "BuiltIns.h"
#include "Effects.h"

void SetSolidColour(RGBPixel Colour) {
    ITERATE_ALL_LEDS(u_int32_t, i) {
        LEDS.SetPixel(i, Colour);
    }
}

void SetSolidColour(HSLPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour,ThisCol);
    SetSolidColour(ThisCol);

}

void SetSolidColour(HSVPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour,ThisCol);
    SetSolidColour(ThisCol);
}

void SetSolidColour(u_int32_t Start, u_int32_t End, RGBPixel Colour) {
    ITERATE_BETWEEN_INC(u_int32_t, i, Start, End){
        LEDS.SetPixel(i,Colour);
    }
}

void SetSolidColour(u_int32_t Start, u_int32_t End, HSLPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour,ThisCol);
    SetSolidColour(Start,End,ThisCol);
}

void SetSolidColour(u_int32_t Start, u_int32_t End, HSVPixel Colour) {
    RGBPixel ThisCol;
    ColourConverter.ToRGB(Colour,ThisCol);
    SetSolidColour(Start,End,ThisCol);
}
