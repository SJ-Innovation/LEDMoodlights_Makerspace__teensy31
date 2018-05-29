
#ifndef RGBConverter_h
#define RGBConverter_h


#include <Arduino.h>


struct RGBPixel {
    u_int8_t R;
    u_int8_t G;
    u_int8_t B;
};

struct HSLPixel {
    float H;
    float S;
    float L;
};

struct HSVPixel {
    float H;
    float S;
    float V;
};

class RGBConverter_t {

public:
    void ToHSL(RGBPixel &RGB, HSLPixel &HSL);
    void ToHSL(HSVPixel &HSV, HSLPixel &HSL);

    void ToHSV(RGBPixel &RGB, HSVPixel &HSV);
    void ToHSV(HSLPixel &HSL, HSVPixel &HSV);

    void ToRGB(HSLPixel &HSL, RGBPixel &RGB);
    void ToRGB(HSVPixel &HSV, RGBPixel &RGB);

private:
    float MaxThree(float a, float b, float c);

    float MinThree(float a, float b, float c);

    float HueToRGB(float p, float q, float t);
};



#endif
