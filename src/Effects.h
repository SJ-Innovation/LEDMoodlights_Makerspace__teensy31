

#ifndef _EFFECTS_H
#define _EFFECTS_H

#include "Arduino.h"
#include "Audio.h"
#include "SuperFastNeoPixel.h"
#include "Config.h"
#include "SoundAnalysis.h"
#include "RGBConverter_t.h"
#include "BuiltIns.h"
#include "lib8tion.h"

void RegisterCustomEffects();
void PostCall();
void PreCall();
extern RGBConverter_t ColourConverter;

extern bool RegisterNextEffect(float DisplayTime, float Frequency, void (*EffectFunction)(void));

extern bool RegisterEffectFunctionTo(int Index, float Frequency, float DisplayTime, void (*EffectFunction)(void));

extern AudioAnalyzeFFT1024 FFTChannel;
extern AudioAnalyzeRMS RMSChannel;
extern AudioAnalyzePeak PeakChannel;
extern SuperFastNeoPixel LEDS;
extern SoundAnalyser_t SoundAnalyser;
#endif //_EFFECTS_H
