

#include "CustomEffects.h"


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

 Pixel Types:
    Raw u_int32_t 0xRRGGBB - RGB in single number form. (0-255 RGB)
    RGBPixel .R .G .B - RGB u_int_8 form. (0-255)
    HSLPixel .H .S .L - HSL float form (0-1)
    HSVPixel .H .S .V - HSV float form (0-1)
*/




void TestEffect1();
void Rainbow();
void RainbowWithGlitter();
void Confetti();
void SineLon();
void BPM();
void Juggle();

void RegisterCustomEffects() { // Called during init to assign and set up all effect functions.
    LEDS.SetGlobalBrightness(0.1);
    RegisterNextEffect(50,3,Rainbow);
  //  RegisterNextEffect(10,3,RainbowWithGlitter);
  //  RegisterNextEffect(20,3,Confetti);
  //  RegisterNextEffect(30,3,SineLon);
  //  RegisterNextEffect(5,3,BPM);
  //  RegisterNextEffect(50,3,Juggle);
}
float EffectVarietyCounter = 0; // rotating "base color" used by many of the patterns
void PreCall(){

}

void PostCall(){
    EffectVarietyCounter+=0.01;

}


void TestEffect1() {
    if (SoundAnalyser.BeatHappened()) {
        digitalWriteFast(LED_BUILTIN, HIGH);
        delay(10);
        digitalWriteFast(LED_BUILTIN, LOW);
    }
}




void Rainbow() {
    // FastLED's built-in Rainbow generator
    FillRainbow(0, NUMBER_OF_LEDS, EffectVarietyCounter, 0.05);
}

void AddGlitter(u_int8_t ChanceOfGlitter) {
    if (random8() < ChanceOfGlitter) {
        LEDS.SetPixel(random16(NUMBER_OF_LEDS), 255, 255, 255);
    }
}

void RainbowWithGlitter() {
    // built-in FastLED Rainbow ported over, plus some random sparkly glitter
    Rainbow();
    AddGlitter(80);
}


void Confetti() {
    // random colored speckles that blink in and fade smoothly
    Fade(0, NUMBER_OF_LEDS, 0.05);
    u_int32_t Pos = random16(NUMBER_OF_LEDS);
    LEDS.SetPixel(Pos, EffectVarietyCounter + random8(64), 200, 255);

}

void SineLon() {
    // a colored dot sweeping back and forth, with fading trails
    Fade(0, NUMBER_OF_LEDS, 0.1);
    int Pos = beatsin16(13, 0, NUMBER_OF_LEDS - 1);
    LEDS.SetPixel(Pos, EffectVarietyCounter, 255, 192);

}

//#include "FastLED.h"

void BPM() {
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    uint8_t Beat = beatsin8(BeatsPerMinute, 0, 15);
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        LEDS.SetPixel(i, ReadFromPallete(PartyColors_Pallete,Beat+i));
    }
}

void Juggle() {
    // eight colored dots, weaving in and out of sync with each other
    Fade(0, NUMBER_OF_LEDS, 0.05);
    float DotHue = 0;
    for (int i = 0; i < 8; i++) {
        HSVPixel ThisPix = {DotHue, 0.75, 1};
        LEDS.SetPixel(beatsin16(i + 7, 0, NUMBER_OF_LEDS - 1),ThisPix);
        DotHue += 0.125;
    }
}
//
//






