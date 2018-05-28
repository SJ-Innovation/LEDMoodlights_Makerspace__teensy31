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


#ifndef SuperFastNeoPixel_h_
#define SuperFastNeoPixel_h_

#include <Arduino.h>
#include "DMAChannel.h"

enum PIXELORDER {
    RGB = 0,// The WS2811 datasheet documents this way
    RBG = 1,
    GRB = 2, // Most LED strips are wired this way
    GBR = 3,
    BRG = 4,
    BGR = 5
};
enum COLOURS {
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    YELLOW = 0xFFFF00,
    PINK = 0xFF1088,
    ORANGE = 0xE05800,
    WHITE = 0xFFFFFF
};

class SuperFastNeoPixel {
public:
    constexpr SuperFastNeoPixel(uint16_t num, void *fb, void *db, uint8_t pin, uint8_t cfg) :
            NumLed(num), Pin(pin), Config(cfg),
            FrameBuffer((uint8_t *) fb), DrawBuffer((uint8_t *) db) {
    }

    bool Begin();

    void SetPixel(uint32_t num, int Colour) {
        if (num >= NumLed) return;
        num *= 3;
        DrawBuffer[num + 0] = Colour & 255;
        DrawBuffer[num + 1] = (Colour >> 8) & 255;
        DrawBuffer[num + 2] = (Colour >> 16) & 255;
    }

    void SetPixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue) {
        if (num >= NumLed) return;
        num *= 3;
        DrawBuffer[num + 0] = blue;
        DrawBuffer[num + 1] = green;
        DrawBuffer[num + 2] = red;
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
