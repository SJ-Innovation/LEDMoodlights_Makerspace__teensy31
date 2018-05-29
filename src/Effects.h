

#ifndef _EFFECTS_H
#define _EFFECTS_H

#include "Arduino.h"
#include "Audio.h"
#include "SuperFastNeoPixel.h"
#include "Config.h"
#include "SoundAnalysis.h"
#include "RGBConverter_t.h"
#include "BuiltIns.h"



void RegisterCustomEffects();
extern RGBConverter_t ColourConverter;

extern bool RegisterNextEffect(int DisplayTime, int Frequency, void (*EffectFunction)(void));

extern bool RegisterEffectFunctionTo(int Index, int Frequency, int DisplayTime, void (*EffectFunction)(void));

extern AudioAnalyzeFFT1024 FFTChannel;
extern AudioAnalyzeRMS RMSChannel;
extern AudioAnalyzePeak PeakChannel;
extern SuperFastNeoPixel LEDS;
extern SoundAnalyser_t SoundAnalyser;
#endif //WS2811DRIVER_TEENSY31_EFFECTS_H
