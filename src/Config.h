#ifndef WS2811DRIVER_TEENSY31_CONFIG_H
#define WS2811DRIVER_TEENSY31_CONFIG_H

#define AUDIO_IN_PIN A2
#define LED_OUT_PIN 1
#define NUMBER_OF_LEDS 215 // TODO GET CORRECT NUMBER

#define MAX_EFFECT_FUNCTIONS 10

#define DEFAULT_AverageFrames 200      // ticks to average audio energy for comparison against instance energy //
#define DEFAULT_BeatWaitInterval 400  // time to wait for detection after a beat
#define DEFAULT_BeatHoldTime 80       // number of frames to hold a beat
#define DEFAULT_BeatDecayRate  0.99   // how much to decay the beat threshold on a non-beat
#define DEFAULT_BeatMin 0.25           // anything with a peak level lower than this will not count as a beat

#define FAST_AS_POSSIBLE 150




#endif //WS2811DRIVER_TEENSY31_CONFIG_H
