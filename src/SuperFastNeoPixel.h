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
            _NumLed(num), Pin(pin), Config(cfg),
            FrameBuffer((uint8_t *) fb), DrawBuffer((uint8_t *) db) {
        _Brightness = 1;
    }

    bool Begin();

    void ScalePixel(u_int32_t Num, float ScaleFactor) {
        if (Num >= _NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] *= ScaleFactor;
        DrawBuffer[Num + 1] *= ScaleFactor;
        DrawBuffer[Num + 2] *= ScaleFactor;
    }

    void AddToPixel(u_int32_t Num, u_int8_t r, u_int8_t g, u_int8_t b) {
        if (Num >= _NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] += min(r, 255 - DrawBuffer[Num + 0]);
        DrawBuffer[Num + 1] += min(g, 255 - DrawBuffer[Num + 1]);
        DrawBuffer[Num + 2] += min(b, 255 - DrawBuffer[Num + 2]);
    }

    void AddToPixel(u_int32_t Num, u_int32_t Offset) {
        if (Num >= _NumLed) return;
        AddToPixel(Num, (Offset >> 16) & 255, (Offset >> 8) & 255, Offset & 255);
    }

    void AddToPixel(u_int32_t Num, RGBPixel &Offset) {
        if (Num >= _NumLed) return;
        AddToPixel(Num, Offset.R, Offset.G, Offset.B);
    }

    void AddToPixel(u_int32_t Num, HSVPixel &Offset) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Offset, Temp);
        AddToPixel(Num, Temp);
    }

    void AddToPixel(u_int32_t Num, HSLPixel &Offset) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Offset, Temp);
        AddToPixel(Num, Temp);
    }

    void SubtractFromPixel(u_int32_t Num, u_int8_t r, u_int8_t g, u_int8_t b) {
        if (Num >= _NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] -= min(r, DrawBuffer[Num + 0]);
        DrawBuffer[Num + 1] -= min(g, DrawBuffer[Num + 1]);
        DrawBuffer[Num + 2] -= min(b, DrawBuffer[Num + 2]);
    }

    void SubtractFromPixel(u_int32_t Num, RGBPixel &Offset) {
        if (Num >= _NumLed) return;
        SubtractFromPixel(Num, Offset.R, Offset.G, Offset.B);
    }

    void SubtractFromPixel(u_int32_t Num, HSVPixel &Offset) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Offset, Temp);
        SubtractFromPixel(Num, Temp);
    }

    void SubtractFromPixel(u_int32_t Num, HSLPixel &Offset) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Offset, Temp);
        SubtractFromPixel(Num, Temp);
    }

    void SetPixel(u_int32_t Num, uint8_t red, uint8_t green, uint8_t blue) {
        if (Num >= _NumLed) return;
        Num *= 3;
        DrawBuffer[Num + 0] = red;
        DrawBuffer[Num + 1] = green;
        DrawBuffer[Num + 2] = blue;
    }

    void SetPixel(u_int32_t Num, u_int32_t Colour) {
        if (Num >= _NumLed) return;
        SetPixel(Num, (Colour >> 16) & 255, (Colour >> 8) & 255, Colour & 255);
    }


    void SetPixel(u_int32_t Num, RGBPixel &Pixel) {
        if (Num >= _NumLed) return;
        SetPixel(Num, Pixel.R, Pixel.G, Pixel.B);
    }

    void SetPixel(u_int32_t Num, HSLPixel &Pixel) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Pixel, Temp);
        SetPixel(Num, Temp);
    }

    void SetPixel(u_int32_t Num, HSVPixel &Pixel) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        ColourConverter.ToRGB(Pixel, Temp);
        SetPixel(Num, Temp);
    }

    u_int32_t GetPixel(u_int32_t Num) {
        if (Num >= _NumLed) return 0;
        Num *= 3;
        return 0xFFFFFF & ((DrawBuffer[Num] << 16) | (DrawBuffer[Num + 1] << 8) | (DrawBuffer[Num + 2]));
    }

    void GetPixel(u_int32_t Num, RGBPixel &StoreTo) {
        if (Num >= _NumLed) return;
        Num *= 3;
        StoreTo.R = DrawBuffer[Num];
        StoreTo.G = DrawBuffer[Num + 1];
        StoreTo.B = DrawBuffer[Num + 2];
    }

    void GetPixel(u_int32_t Num, HSLPixel &StoreTo) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        GetPixel(Num, Temp);
        ColourConverter.ToHSL(Temp, StoreTo);
    }

    void GetPixel(u_int32_t Num, HSVPixel &StoreTo) {
        if (Num >= _NumLed) return;
        RGBPixel Temp;
        GetPixel(Num, Temp);
        ColourConverter.ToHSV(Temp, StoreTo);
    }

    void Clear() {
        memset(DrawBuffer, 0, _NumLed * 3);
    }

    void SetGlobalBrightness(float NewBrightness) {
        NewBrightness = constrain(NewBrightness, 0, 1);
        _Brightness = NewBrightness;
    }

    float GetGlobalBrightness() {
        return _Brightness;
    }


    void ShowBlocking();

    void ShowNonBlocking();

    bool IsBusy();

    uint16_t NumPixels() {
        return _NumLed;
    }

    int GetUpdateRate();

private:
    const uint16_t _NumLed;
    const uint8_t Pin;
    const uint8_t Config;
    uint8_t *FrameBuffer;
    uint8_t *DrawBuffer;
    DMAChannel *DMA = nullptr;
    uint32_t Prior_Micros = 0;
    u_int32_t FrameCount = 0;
    float _Brightness = 1;
};

#endif
