

#ifndef WS2811DRIVER_TEENSY31_EFFECTS_H
#define WS2811DRIVER_TEENSY31_EFFECTS_H

#include "Arduino.h"
#include "Audio.h"
#include "SuperFastNeoPixel.h"
#include "Config.h"
#include "BeatDetection.h"

void RegisterCustomEffects();

extern bool RegisterNextEffect(int DisplayTime, int Frequency, void (*EffectFunction)(void));

extern bool RegisterEffectFunctionTo(int Index, int Frequency, int DisplayTime, void (*EffectFunction)(void));

extern AudioAnalyzeFFT1024 FFTChannel;
extern SuperFastNeoPixel LEDS;
extern BeatAnalyzer BeatDetector;
#endif //WS2811DRIVER_TEENSY31_EFFECTS_H
