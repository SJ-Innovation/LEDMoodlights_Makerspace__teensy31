
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
    void ToHSL(u_int32_t Raw, HSLPixel &HSL);
    HSLPixel ToHSL(RGBPixel &RGB);
    HSLPixel ToHSL(HSVPixel &HSV);
    HSLPixel ToHSL(u_int32_t &Raw);

    void ToHSV(RGBPixel &RGB, HSVPixel &HSV);
    void ToHSV(HSLPixel &HSL, HSVPixel &HSV);
    void ToHSV(u_int32_t Raw, HSVPixel &HSV);
    HSVPixel ToHSV(RGBPixel &RGB);
    HSVPixel ToHSV(HSLPixel &HSL);
    HSVPixel ToHSV(u_int32_t &Raw);

    void ToRGB(HSLPixel &HSL, RGBPixel &RGB);
    void ToRGB(HSVPixel &HSV, RGBPixel &RGB);
    void ToRGB(u_int32_t Raw, RGBPixel &RGB);
    RGBPixel ToRGB(HSLPixel &HSL);
    RGBPixel ToRGB(HSVPixel &HSV);
    RGBPixel ToRGB(u_int32_t &Raw);

    void ToRaw(RGBPixel &RGB, u_int32_t &StoreTo);
    void ToRaw(HSLPixel &HSL, u_int32_t &StoreTo);
    void ToRaw(HSVPixel &HSV, u_int32_t &StoreTo);
    u_int32_t ToRaw(RGBPixel &RGB);
    u_int32_t ToRaw(HSLPixel &HSL);
    u_int32_t ToRaw(HSVPixel &HSV);

private:
    float MaxThree(float a, float b, float c);

    float MinThree(float a, float b, float c);

    float HueToRGB(float p, float q, float t);
};



#endif
