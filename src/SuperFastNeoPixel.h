//## Supported Pins & Serial Ports
//
//| Port    | Teensy LC   | Teensy 3.2 | Teensy 3.5 | Teensy 3.6 |
//| :------ | :---------: | :--------: | :--------: | :--------: |
//| Serial1 | 1, 4, 5, 24 | 1, 5       | 1, 5, 26   | 1, 5, 26   |
//| Serial2 |             | 10, 31     | 10         | 10         |
//| Serial3 |             | 8, 20      | 8, 20      | 8, 20      |
//| Serial4 |             |            | 32         | 32         |
//| Serial5 |             |            | 33         | 33         |
//| Serial6 |             |            | 48         |            |


#ifndef _SUPER_FAST_NEO_PIXEL_H_
#define _SUPER_FAST_NEO_PIXEL_H_

#include <Arduino.h>
#include "DMAChannel.h"
#include "RGBConverter_t.h"


enum PIXELORDER {
    RGB = 0,// The WS2811 datasheet documents this way
    RBG = 1,
    GRB = 2, // Most LED strips are wired this way
    GBR = 3,
    BRG = 4,
    BGR = 5
};

extern RGBConverter_t ColourConverter;







//enum class COLOURS {
//    RED = Colour_t(0xFF0000),
//    GREEN = 0x00FF00,
//    BLUE = 0x0000FF,
//    YELLOW = 0xFFFF00,
//    PINK = 0xFF1088,
//    ORANGE = 0xE05800,
//    WHITE = 0xFFFFFF
//};


class SuperFastNeoPixel {
public:
    constexpr SuperFastNeoPixel(uint16_t num, void *fb, void *db, uint8_t pin, uint8_t cfg) :
            NumLed(num), Pin(pin), Config(cfg),
            FrameBuffer((uint8_t *) fb), DrawBuffer((uint8_t *) db) {
    }

    bool Begin();

    void SetPixel(u_int32_t Num, u_int32_t Colour) {
        if (Num >= NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] = Colour & 255;
        DrawBuffer[Num + 1] = (Colour >> 8) & 255;
        DrawBuffer[Num + 2] = (Colour >> 16) & 255;

    }

    void SetPixel(u_int32_t Num, uint8_t red, uint8_t green, uint8_t blue) {
        if (Num >= NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] = blue;
        DrawBuffer[Num + 1] = green;
        DrawBuffer[Num + 2] = red;
    }

    void SetPixel(u_int32_t Num, RGBPixel &Pixel) {
        if (Num >= NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] = Pixel.R;
        DrawBuffer[Num + 1] = Pixel.G;
        DrawBuffer[Num + 2] = Pixel.B;
    }

    void SetPixel(u_int32_t Num, HSLPixel &Pixel) {
        if (Num >= NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Pixel, Temp);
        SetPixel(Num, Temp);
    }

    void SetPixel(u_int32_t Num, HSVPixel &Pixel) {
        if (Num >= NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Pixel, Temp);
        SetPixel(Num, Temp);
    }

    u_int32_t GetPixel(u_int32_t Num) {
        return 0xFFFFFF & ((DrawBuffer[Num] << 16) | (DrawBuffer[Num + 1] << 8) | (DrawBuffer[Num + 2]));
    }

    void GetPixel(u_int32_t Num, RGBPixel &StoreTo) {
        if (Num >= NumLed) return;
        Num *= 3;
        StoreTo.R = DrawBuffer[Num];
        StoreTo.G = DrawBuffer[Num + 1];
        StoreTo.B = DrawBuffer[Num + 2];
    }

    void GetPixel(u_int32_t Num, HSLPixel &StoreTo) {
        if (Num >= NumLed) return;
        Num *= 3;
        RGBPixel Temp;
        GetPixel(Num, Temp);
        ColourConverter.ToHSL(Temp, StoreTo);
    }

    void GetPixel(u_int32_t Num, HSVPixel &StoreTo) {
        if (Num >= NumLed) return;
        Num *= 3;
        RGBPixel Temp;
        GetPixel(Num, Temp);
        ColourConverter.ToHSV(Temp, StoreTo);
    }

    void Clear() {
        memset(DrawBuffer, 0, NumLed * 3);
    }

    void ShowBlocking();

    void ShowNonBlocking();

    bool IsBusy();

    uint16_t NumPixels() {
        return NumLed;
    }

    int GetUpdateRate();

private:
    const uint16_t NumLed;
    const uint8_t Pin;
    const uint8_t Config;
    uint8_t *FrameBuffer;
    uint8_t *DrawBuffer;
    DMAChannel *DMA = nullptr;
    uint32_t Prior_Micros = 0;
    u_int32_t FrameCount = 0;
};

#endif
