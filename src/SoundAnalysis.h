

#ifndef _SOUND_ANALYSIS_H
#define _SOUND_ANALYSIS_H

#include "Audio.h"
// Beat Detector
// Track a threshold volume level.
// If the current volume exceeds the threshold then you have a beat. Set the new threshold to the current volume.
// Reduce the threshold over time, using the Decay Rate.
// Wait for the Hold Time before detecting for the next beat. This can help reduce false positives.

class SoundAnalyser_t {
public:
    SoundAnalyser_t(AudioAnalyzeFFT1024 *, AudioAnalyzePeak *, AudioAnalyzeRMS *);

    ~SoundAnalyser_t();

    void Update();


    float GetDominantFrequency();

    float GetVolumePeak();

    float GetVolumeRMS();

    uint16_t BeatHappened();

    void BeatSetAverageFrames(u_int32_t New) {
        AverageFrames = New;
    }

    void BeatSetWaitInterval(u_int32_t New) {
        BeatWaitInterval = New;
    }

    void BeatSetHoldTime(u_int32_t New) {
        BeatHoldTime = New;
    }

    void BeatSetDecayRate(float New) {
        BeatDecayRate = New;
    }

    void BeatSetMin(float New) {
        BeatMin = New;
    }

    AudioAnalyzeFFT1024 *FFTAnalyser;
    AudioAnalyzePeak *PeakAnalyser;
    AudioAnalyzeRMS *RMSAnalyser;
private:


    float _DominantFreq;
    float _VolumePeak;
    float _VolumeRMS;

    float AvgPeak;
    float PeakRunningTotal = 0.0;
    u_int32_t NumFramesCounted = 0;
    u_int32_t Timer = 0;
    int NumBeats;
    u_int32_t AverageFrames;
    u_int32_t BeatWaitInterval;
    u_int32_t BeatHoldTime;
    float BeatDecayRate;
    float BeatMin;
    float BeatCutoff = 0.5;
    u_int32_t BeatTime = 0;

    void _UpdateDominantFreq();

    void _UpdateBeatDetector();
};

#endif //WS2811DRIVER_TEENSY31_FREQUENCYANALYSIS_H
