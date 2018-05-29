#include <Arduino.h>
#include <Audio.h>
#include "SuperFastNeoPixel.h"
#include "Config.h"
#include "Effects.h"

#include "SoundAnalysis.h"

AudioInputAnalogStereo InputChannel(AUDIO_IN1_PIN, AUDIO_IN2_PIN);

AudioMixer4 InputMixer;
AudioConnection InputCord1(InputChannel, 0, InputMixer, 0);
AudioConnection InputCord2(InputChannel, 1, InputMixer, 1);

AudioAnalyzeFFT1024 FFTChannel;
AudioAnalyzePeak PeakChannel;
AudioAnalyzeRMS RMSChannel;

AudioConnection Stream1(InputMixer, FFTChannel);
AudioConnection Stream2(InputMixer, PeakChannel);
AudioConnection Stream3(InputMixer, RMSChannel);

SoundAnalyser_t SoundAnalyser(&FFTChannel, &PeakChannel, &RMSChannel);
RGBConverter_t ColourConverter;

byte DrawBuffer[NUMBER_OF_LEDS * 3];
DMAMEM byte DisplayBuffer[NUMBER_OF_LEDS * 12];
SuperFastNeoPixel LEDS(NUMBER_OF_LEDS, DisplayBuffer, DrawBuffer, LED_OUT_PIN, PIXELORDER::BRG); // Set up for rendering to the LEDs

#define IN_RANGE(Min, X, Max) ((X>=Min && X<= Max)? 1:0)

void (*EffectFunctions[MAX_EFFECT_FUNCTIONS])(void) = { }; // Registry for effect functions.
u_int32_t EffectRecallTimes[MAX_EFFECT_FUNCTIONS] = { };
u_int32_t EffectIntervalTimes[MAX_EFFECT_FUNCTIONS] = { };

void NullEffectFunction() { // Required for blank effects to reduce risk of seg.

}

void BlankRegisterTable() { // Initialise table and blank to nulleffect
    for (int i = 0; i < MAX_EFFECT_FUNCTIONS; i++) {
        EffectRecallTimes[i] = 0;
        EffectFunctions[i] = &NullEffectFunction;
        EffectIntervalTimes[i] = 0;
    }
}


extern "C" char *sbrk(int incr);

int FreeRam() { // Difference between stack and heap pointers.
    char top;
    return &top - reinterpret_cast<char *>(sbrk(0));
}

bool RegisterEffectFunctionTo(int Index, int Frequency, int DisplayTime,
                              void (*EffectFunction)(void)) { // Registers an effect function to the set registry index. Overwrites.
    Frequency = min(Frequency, FAST_AS_POSSIBLE);
    if (IN_RANGE(0, Index, MAX_EFFECT_FUNCTIONS - 1) && IN_RANGE(0, Frequency, FAST_AS_POSSIBLE)) {
        EffectFunctions[Index] = EffectFunction;
        EffectRecallTimes[Index] = DisplayTime;
        if (Frequency == FAST_AS_POSSIBLE) {
            EffectIntervalTimes[Index] = 0;
        }
        else {
            EffectIntervalTimes[Index] = round(1000.0F / (float) Frequency);
        }
        Serial.print(F("EFFECT REGISTER PASSED: "));
        Serial.println(Index);
        return true;
    }
    Serial.print(F("EFFECT REGISTER FAILED: "));
    Serial.println(Index);
    return false;
}

bool RegisterNextEffect(int Frequency, int DisplayTime, void (*EffectFunction)(void)) { // Registers an effect to the next available function index.
    static int CurrentFunctionRegister = 0;
    return RegisterEffectFunctionTo(CurrentFunctionRegister++, Frequency, DisplayTime, EffectFunction);
}

int GetEffectIndex(void(*EffectFunction)(void)) {
    for (int i = 0; i < MAX_EFFECT_FUNCTIONS; i++) {
        if (EffectFunctions[i] == EffectFunction) {
            return i;
        }
    }
    Serial.println(F("WARNING: Invalid Effect Function Index Searched For."));
    return -1;
}

int ForceNextEffectIndex = -1;
bool ForceNextEffectImmediate = false;

void ForceNextEffectTo(int Index, bool Immediate) { // Forces the next event to be the first occourance of the given event index.
    Index = max(0, Index);
    Index = min(MAX_EFFECT_FUNCTIONS - 1, Index);
    ForceNextEffectImmediate = Immediate;
    ForceNextEffectIndex = Index;
}

void ForceNextEffect(void(*EffectFunction)(void), bool Immediate) { // Forgoes next effect in line and forces next effect to be the first instance of the one given.
    ForceNextEffectTo(GetEffectIndex(EffectFunction), Immediate);
}



void setup() {
    pinMode(LED_BUILTIN, OUTPUT_STRONGDRIVE);
    pinMode(LED_OUT_PIN, OUTPUT_STRONGDRIVE);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    AudioMemory(15);
    InputMixer.gain(0, 1);
    InputMixer.gain(1, 1);
    LEDS.Begin();
    BlankRegisterTable();
    RegisterCustomEffects(); // In Custom Effects file, registers users effects.

}


void loop() {
    u_int32_t Now = millis();
    static int CurrentEffectIndex = 0;
    static u_int32_t LastEffectChange = 0;
    static u_int32_t LastEffectFunctionRun = 0;

    if (Now - LastEffectChange >= EffectRecallTimes[CurrentEffectIndex] or ForceNextEffectImmediate) { // If its time to change effect.

        if (ForceNextEffectIndex == -1) {
            do {
                CurrentEffectIndex++;
                if (CurrentEffectIndex >= MAX_EFFECT_FUNCTIONS) {
                    CurrentEffectIndex = 0;
                }
            } while (EffectFunctions[CurrentEffectIndex] ==
                     NullEffectFunction); // Search for the next effect that isnt the nulleffect. Keep going until we find one.
        }
        else {
            CurrentEffectIndex = ForceNextEffectIndex;
            ForceNextEffectIndex = -1;
        }
        ForceNextEffectImmediate = false;
        LastEffectChange = Now;
    }

    if ((not LEDS.IsBusy()) and (EffectIntervalTimes[CurrentEffectIndex] == 0 or Now - LastEffectFunctionRun >=
                                                                                 EffectIntervalTimes[CurrentEffectIndex])) { // Is it time to run the effect function as defined by its call frequency?
        LastEffectFunctionRun = Now;
        EffectFunctions[CurrentEffectIndex](); // Run whichever effect we need to run.
        LEDS.ShowNonBlocking(); // Start the LEDs updating first. Its asyncronous.
    }

    SoundAnalyser.Update(); // Update audio handlers.

}