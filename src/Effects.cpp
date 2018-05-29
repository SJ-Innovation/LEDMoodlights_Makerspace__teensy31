

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

*/




void TestEffect1();


void RegisterCustomEffects(){ // Called during init to assign and set up all effect functions.
    RegisterNextEffect(100,1000,TestEffect1);

}

void TestEffect1(){
   if (SoundAnalyser.BeatHappened()){
       digitalWriteFast(LED_BUILTIN,HIGH);
       delay(10);
       digitalWriteFast(LED_BUILTIN,LOW);
   }
}














