#include <Arduino.h>
#include <Audio.h>
#include "SuperFastNeoPixel.h"
#include "Config.h"
#include "Effects.h"
#include "BeatDetection.h"

AudioInputAnalog InputChannel(AUDIO_IN_PIN);

AudioAnalyzeFFT1024 FFTChannel;
AudioConnection Stream1(InputChannel, FFTChannel);

AudioAnalyzePeak PeakDetector;
AudioConnection Stream2(InputChannel, PeakDetector);
BeatAnalyzer BeatDetector(&PeakDetector);


byte DrawBuffer[NUMBER_OF_LEDS * 3];
DMAMEM byte DisplayBuffer[NUMBER_OF_LEDS * 12];
SuperFastNeoPixel LEDS(NUMBER_OF_LEDS, DisplayBuffer, DrawBuffer, LED_OUT_PIN, PIXELORDER::BRG);

#define IN_RANGE(Min, X, Max) ((X>=Min && X<= Max)? 1:0)

void (*EffectFunctions[MAX_EFFECT_FUNCTIONS])(void) = { };

u_int32_t EffectRecallTimes[MAX_EFFECT_FUNCTIONS] = { };
u_int32_t EffectIntervalTimes[MAX_EFFECT_FUNCTIONS] = { };

void NullEffectFunction() {

}

void BlankRegisterTable() {
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

bool RegisterEffectFunctionTo(int Index, int Frequency, int DisplayTime, void (*EffectFunction)(void)) {
    Frequency = min(Frequency,FAST_AS_POSSIBLE);
    if (IN_RANGE(0, Index, MAX_EFFECT_FUNCTIONS - 1) && IN_RANGE(0,Frequency,FAST_AS_POSSIBLE)) {
        EffectFunctions[Index] = EffectFunction;
        EffectRecallTimes[Index] = DisplayTime;
        EffectIntervalTimes[Index] = round(1000.0F/(float)Frequency);
        Serial.print(F("EFFECT REGISTER PASSED: "));
        Serial.println(Index);
        return true;
    }

    Serial.print(F("EFFECT REGISTER FAILED: "));
    Serial.println(Index);
    return false;
}

bool RegisterNextEffect(int Frequency, int DisplayTime, void (*EffectFunction)(void)) {
    static int CurrentFunctionRegister = 0;
    return RegisterEffectFunctionTo(CurrentFunctionRegister++, Frequency, DisplayTime, EffectFunction);
}


void setup() {
    pinMode(LED_BUILTIN, OUTPUT_STRONGDRIVE);
    pinMode(LED_OUT_PIN, OUTPUT_STRONGDRIVE);
    Serial.begin(115200);
    AudioMemory(15);
    LEDS.Begin();
    BlankRegisterTable();
    RegisterCustomEffects();

}

void loop() {
    u_int32_t Now = millis();
    static int CurrentEffectIndex = 0;
    static u_int32_t LastEffectChange = 0;
    static u_int32_t LastEffectFunctionRun = 0;

    if (Now - LastEffectChange >= EffectRecallTimes[CurrentEffectIndex]) {
        do {
            CurrentEffectIndex++;
            CurrentEffectIndex %= MAX_EFFECT_FUNCTIONS;
        } while (EffectFunctions[CurrentEffectIndex] == NullEffectFunction);


        LastEffectChange = Now;
        Serial.print(F("EFFECT CHANGE TO: "));
        Serial.println(CurrentEffectIndex);
        Serial.print(" FOR:");
        Serial.print(EffectRecallTimes[CurrentEffectIndex]);
        Serial.println("ms");
    }

    if (Now - LastEffectFunctionRun >= EffectIntervalTimes[CurrentEffectIndex]) {
        LastEffectFunctionRun = Now;
        EffectFunctions[CurrentEffectIndex]();

    }
    FFTChannel.update();
    BeatDetector.update();
    LEDS.Show();


}