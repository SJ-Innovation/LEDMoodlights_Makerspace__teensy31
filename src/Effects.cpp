

#include "Effects.h"


/* Effect Functions must take no args, and return void.
 They can access the LED, SoundAnalysis [Beat Detection, RMS Volume, Peak Volume, Dominant Frequency]
 Effect functions are called at least once per refresh cycle, followed by an LED refresh, each function is called for the length of time specified in its register call.
 An effect function can block, audio processing will still continue, but should only block for the expected duration of its cycle.
 Registering a function with frequency=FAST_AS_POSSIBLE will call the function as fast as possible. (About 140Hz)
 Effects will be displayed in the order they're registered in.


 We have access to:
    LEDS - Setting the Colours of all of the LEDs etc
        Can set in terms of RGB, HSV, HSL colour schemes. Functions accept all.
        Can get in terms of all above. Functions determined according to type of container given.
        Getting and Setting RGB pixels will be the fastest as no intermediate conversion is needed.
        HSL->HSV is the slowest as it must first transition through RGB.
    BeatDetector - Detects beats in the incoming signal. Tunable.
    FFTChannel - Has 1024 point frequency map of incoming signal
    RMSChannel - Directly access the RMS Analyser
    PeakChannel - Directly access the peak analyser

 Registration Functions:
    RegisterEffectFunctionTo(ID<MAX_EFFECT_FUNCTIONS,CallFrequency(Hz),RepeatTime(S),EffectFunctionPointer(void*void)) - Register a function to a specific ID (overwrites, useful for reassignment)
    RegisterNextEffect(Frequency(Hz), RepeatTime(S),EffectFunctionPointer(void*void)) - Assigns the function to the next place in the table. Useful for init.

 Flow Control:
    ForceNextEffectTo(ID<MAX_EFFECT_FUNCTIONS,Immediate(T/F)) - Forces the next effect to set to the one IDed to. Immediate flag (T- Change ASAP,F-Wait until it wouldve changed naturally). Useful if you know the order.
    ForceNextEffect(EffectFunctionPointer(void*void),Immediate(T/F)) -Forces the next effect to set to the one pointed to. Immediate flag (T- Change ASAP,F-Wait until it wouldve changed naturally). Useful if you dont care about order.

*/




void TestEffect1();
void Rainbow();
void RainbowWithGlitter();
void Confetti();
void SineLon();
void BPM();
void Juggle();

void RegisterCustomEffects() { // Called during init to assign and set up all effect functions.
    RegisterNextEffect(50,3000,Rainbow);
    RegisterNextEffect(50,3000,RainbowWithGlitter);
    RegisterNextEffect(50,3000,Confetti);
    RegisterNextEffect(50,3000,SineLon);
    RegisterNextEffect(50,3000,BPM);
    RegisterNextEffect(50,3000,Juggle);
}

void TestEffect1() {
    if (SoundAnalyser.BeatHappened()) {
        digitalWriteFast(LED_BUILTIN, HIGH);
        delay(10);
        digitalWriteFast(LED_BUILTIN, LOW);
    }
}


uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void Rainbow() {
    // FastLED's built-in Rainbow generator
    FillRainbow(0, NUMBER_OF_LEDS, gHue, 7);
}

void AddGlitter(fract8 ChanceOfGlitter) {
    if (random8() < ChanceOfGlitter) {
        LEDS.AddToPixel(random16(NUMBER_OF_LEDS), 255, 255, 255);
    }
}

void RainbowWithGlitter() {
    // built-in FastLED Rainbow, plus some random sparkly glitter
    Rainbow();
    AddGlitter(80);
}


void Confetti() {
    // random colored speckles that blink in and fade smoothly
    Fade(0, NUMBER_OF_LEDS, 0.05);
    u_int32_t Pos = random16(NUMBER_OF_LEDS);
    LEDS.AddToPixel(Pos, gHue + random8(64), 200, 255);

}

void SineLon() {
    // a colored dot sweeping back and forth, with fading trails
    Fade(0, NUMBER_OF_LEDS, 0.1);
    int Pos = beatsin16(13, 0, NUMBER_OF_LEDS - 1);
    LEDS.AddToPixel(Pos, gHue, 255, 192);

}

//#include "FastLED.h"

void BPM() {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    const u_int32_t *Palette = PartyColors_p;
    uint8_t Beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        LEDS.SetPixel(i, ReadFromPallete(Palette,gHue + (i * 2)) * (Beat - gHue + (i * 10)));
    }
}

void Juggle() {
    // eight colored dots, weaving in and out of sync with each other
    Fade(0, NUMBER_OF_LEDS, 0.1);
    float DotHue = 0;
    for (int i = 0; i < 8; i++) {
        HSVPixel ThisPix = {DotHue, 0.75, 1};
        LEDS.SetPixel(beatsin16(i + 7, 0, NUMBER_OF_LEDS - 1),ThisPix);
      //  LEDS[beatsin16(i + 7, 0, NUMBER_OF_LEDS - 1)] |= CHSV(dothue, 200, 255);
        DotHue += 0.125;
    }
}
//
//






