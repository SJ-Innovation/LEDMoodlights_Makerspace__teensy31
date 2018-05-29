#include "Colours.h"
#include "RGBConverter_t.h"
#include "Effects.h"

u_int32_t ReadFromPallete(const u_int32_t *Pallete, int Index) { // TODO SAVE TO PROGMEM
    Index %= 16;
    u_int32_t This = pgm_read_dword(&Pallete[Index]);
    return This;
}

void ReadFromPallete(const u_int32_t *Pallete, int Index, u_int32_t &StoreTo) {
    StoreTo = ReadFromPallete(Pallete, Index);
}

void ReadFromPallete(const u_int32_t *Pallete, int Index, RGBPixel &StoreTo) {
    ColourConverter.ToRGB(ReadFromPallete(Pallete, Index), StoreTo);
}

void ReadFromPallete(const u_int32_t *Pallete, int Index, HSLPixel &StoreTo) {
    ColourConverter.ToHSL(ReadFromPallete(Pallete, Index), StoreTo);
}

void ReadFromPallete(const u_int32_t *Pallete, int Index, HSVPixel &StoreTo) {
    ColourConverter.ToHSV(ReadFromPallete(Pallete, Index), StoreTo);
}
