#include "RGBConverter_t.h"


void RGBConverter_t::ToRGB(HSLPixel &HSL, RGBPixel &RGB) {
    float r, g, b;

    if (HSL.S == 0) {
        r = g = b = HSL.L; // achromatic
    }
    else {
        float q = HSL.L < 0.5 ? HSL.L * (1 + HSL.S) : HSL.L + HSL.S - HSL.L * HSL.S;
        float p = 2 * HSL.L - q;
        r = RGBConverter_t::HueToRGB(p, q, HSL.H + 1 / 3.0);
        g = RGBConverter_t::HueToRGB(p, q, HSL.H);
        b = RGBConverter_t::HueToRGB(p, q, HSL.H - 1 / 3.0);
    }

    RGB.R = r * 255;
    RGB.G = g * 255;
    RGB.B = b * 255;
}

void RGBConverter_t::ToRGB(HSVPixel &HSV, RGBPixel &RGB) {
    float r = 0, g = 0, b = 0;

    int i = int(HSV.H * 6);
    float f = HSV.H * 6 - i;
    float p = HSV.V * (1 - HSV.S);
    float q = HSV.V * (1 - f * HSV.S);
    float t = HSV.V * (1 - (1 - f) * HSV.S);

    switch (i % 6) {
        case 0:
            r = HSV.V, g = t, b = p;
            break;
        case 1:
            r = q, g = HSV.V, b = p;
            break;
        case 2:
            r = p, g = HSV.V, b = t;
            break;
        case 3:
            r = p, g = q, b = HSV.V;
            break;
        case 4:
            r = t, g = p, b = HSV.V;
            break;
        case 5:
            r = HSV.V, g = p, b = q;
            break;
    }

    RGB.R = r * 255;
    RGB.G = g * 255;
    RGB.B = b * 255;
}

void RGBConverter_t::ToHSL(RGBPixel &RGB, HSLPixel &HSL) {
    float rd = (float) RGB.R / 255;
    float gd = (float) RGB.G / 255;
    float bd = (float) RGB.B / 255;
    float max = MaxThree(rd, gd, bd);
    float min = MinThree(rd, gd, bd);
    float h = 0, s = 0, l = (max + min) / 2;

    if (max == min) {
        h = s = 0; // achromatic
    }
    else {
        float d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        if (max == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        }
        else if (max == gd) {
            h = (bd - rd) / d + 2;
        }
        else if (max == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }
    HSL.H = h;
    HSL.S = s;
    HSL.L = l;
}

void RGBConverter_t::ToHSL(HSVPixel &HSV, HSLPixel &HSL) {
    RGBPixel Temp;
    ToRGB(HSV, Temp);
    ToHSL(Temp, HSL);
}


void RGBConverter_t::ToHSV(RGBPixel &RGB, HSVPixel &HSV) {
    float rd = (float) RGB.R / 255;
    float gd = (float) RGB.G / 255;
    float bd = (float) RGB.B / 255;
    float max = MaxThree(rd, gd, bd), min = MinThree(rd, gd, bd);
    float h = 0, s = 0, v = max;

    float d = max - min;
    s = max == 0 ? 0 : d / max;

    if (max == min) {
        h = 0; // achromatic
    }
    else {
        if (max == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        }
        else if (max == gd) {
            h = (bd - rd) / d + 2;
        }
        else if (max == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }

    HSV.H = h;
    HSV.S = s;
    HSV.V = v;
}

void RGBConverter_t::ToHSV(HSLPixel &HSL, HSVPixel &HSV) {
    RGBPixel Temp;
    ToRGB(HSL, Temp);
    ToHSV(Temp, HSV);
}

float RGBConverter_t::MaxThree(float a, float b, float c) {
    return max(a, max(b, c));
}

float RGBConverter_t::MinThree(float a, float b, float c) {
    return min(a, min(b, c));
}

float RGBConverter_t::HueToRGB(float p, float q, float t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1 / 6.0) return p + (q - p) * 6 * t;
    if (t < 1 / 2.0) return q;
    if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
    return p;
}
