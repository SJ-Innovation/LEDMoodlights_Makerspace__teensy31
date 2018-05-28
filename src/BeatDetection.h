

#ifndef WS2811DRIVER_TEENSY31_BEATDETECTION_H
#define WS2811DRIVER_TEENSY31_BEATDETECTION_H

#include <Arduino.h>
#include <Audio.h>
#include "Config.h"

//Track a threshold volume level.
//If the current volume exceeds the threshold then you have a beat. Set the new threshold to the current volume.
//Reduce the threshold over time, using the Decay Rate.
//Wait for the Hold Time before detecting for the next beat. This can help reduce false positives.

class BeatAnalyzer {
public:
    BeatAnalyzer(AudioAnalyzePeak *_peakAnalyzer) {
        AverageFrames = DEFAULT_AverageFrames;
        BeatWaitInterval = DEFAULT_BeatWaitInterval;
        BeatHoldTime = DEFAULT_BeatHoldTime;
        BeatDecayRate = DEFAULT_BeatDecayRate;
        BeatMin = DEFAULT_BeatMin;

        Analyzer = _peakAnalyzer;
        NumBeats = 0;
    }

    ~BeatAnalyzer() {
    }

    void SetAverageFrames(u_int32_t New) {
        AverageFrames = New;
    }

    void SetWaitInterval(u_int32_t New) {
        BeatWaitInterval = New;
    }

    void SetHoldTime(u_int32_t New) {
        BeatHoldTime = New;
    }

    void SetDecayRate(float New) {
        BeatDecayRate = New;
    }

    void SetMin(float New) {
        BeatMin = New;
    }

    void update() {
        if (Analyzer->available()) {
            float peak = Analyzer->read();

            // calculate an average of the last few frames of
            // audio data -- anything greater than this AND the running beat threshold
            // will count as a beat
            if (NumFramesCounted >= AverageFrames) {
                AvgPeak = PeakRunningTotal / AverageFrames;
                PeakRunningTotal = 0;
                NumFramesCounted = 0;

//            if(AvgPeak > 0.1) {
//              Serial.print("Average: ");
//              Serial.println(AvgPeak);
//            }
            }

            PeakRunningTotal += peak;
            NumFramesCounted++;

            boolean doDetectBeat = (millis() - timer > BeatWaitInterval);
            if (doDetectBeat && peak > BeatCutoff && peak > BeatMin && peak > AvgPeak) {
                // got beat //
//                Serial.print("BEAT: ");
//                Serial.println(peak);
                NumBeats++;

                // slightly increase beat threshold //
                BeatCutoff = peak * 1.1;

                // reset timer //
                timer = millis();
                BeatTime = 0;
            }
            else {
                if (BeatTime <= BeatHoldTime) {
                    BeatTime++;
                }
                else {
                    // slowly decrease beat cutoff with time //
                    BeatCutoff *= BeatDecayRate;
                    BeatCutoff = max(BeatCutoff, BeatMin);
                }
            }
        }
    }

    uint16_t BeatHappened() {
        uint16_t n = NumBeats;
        NumBeats = 0;
        return n;
    }

private:
    AudioAnalyzePeak *Analyzer;
    float AvgPeak;
    float PeakRunningTotal = 0.0;
    u_int32_t NumFramesCounted = 0;
    u_int32_t timer = 0;
    int NumBeats;

    u_int32_t AverageFrames;
    u_int32_t BeatWaitInterval;
    u_int32_t BeatHoldTime;
    float BeatDecayRate;
    float BeatMin;

    float BeatCutoff = 0.5;
    u_int32_t BeatTime = 0;
};


#endif //WS2811DRIVER_TEENSY31_BEATDETECTION_H
