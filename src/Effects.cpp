

#include "Effects.h"


/* Effect Functions must take no args, and return void.
 They can access the LED, FFT data and Beat Detection.
 Effect functions are called once per refresh cycle, followed by an LED refresh, each function is called for the length of time specified in its register call.
 An effect function can block, audio processing will still continue, but should only block for the expected duration of its cycle.
 Registering a function with frequency=FAST_AS_POSSIBLE will call the function as fast as possible. (About 140Hz)
 Effects will be displayed in the order theyre registered in.


 We have access to:
    LEDS - Setting the Colours of all of the LEDs etc
    BeatDetector - Detects beats in the incoming signal. Tunable.
    FFTChannel - Has 1024 point frequency map of incoming signal

*/




void TestEffect1();
void TestEffect2();

void RegisterCustomEffects(){ // Called during init to assign and set up all effect functions.
    RegisterNextEffect(FAST_AS_POSSIBLE,1000,TestEffect1);
    RegisterNextEffect(50,1000,TestEffect1);
}

void TestEffect1(){

}

void TestEffect2(){

}











