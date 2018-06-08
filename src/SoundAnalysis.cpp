

#include "SoundAnalysis.h"
#include "Config.h"

#define BIN_SEPERATION 43.0F

SoundAnalyser_t::SoundAnalyser_t(AudioAnalyzeFFT1024 *_ThisFFT, AudioAnalyzePeak *_ThisPeak, AudioAnalyzeRMS *_ThisRMS) {
    FFTAnalyser = _ThisFFT;
    PeakAnalyser = _ThisPeak;
    RMSAnalyser = _ThisRMS;
    AverageFrames = DEFAULT_AverageFrames;
    BeatWaitInterval = DEFAULT_BeatWaitInterval;
    BeatHoldTime = DEFAULT_BeatHoldTime;
    BeatDecayRate = DEFAULT_BeatDecayRate;
    BeatMin = DEFAULT_BeatMin;
    NumBeats = 0;
}

SoundAnalyser_t::~SoundAnalyser_t() {

}

void SoundAnalyser_t::Update() {
    FFTAnalyser->update();
    RMSAnalyser->update();
    PeakAnalyser->update();
    _UpdateDominantFreq();
    _UpdateBeatDetector();
}

void SoundAnalyser_t::_UpdateDominantFreq() {
    if (FFTAnalyser->available()) {
        int MaxVal = 0;
        int MaxBin = 0;
        for (int i = 0; i < 512; i++) {
            u_int16_t ThisVal = FFTAnalyser->output[i];
            if (ThisVal > MaxVal) {
                MaxBin = i;
                MaxVal = ThisVal;
            }
        }
        _DominantFreq = MaxBin * BIN_SEPERATION;
    }
}

void SoundAnalyser_t::_UpdateBeatDetector() {
    if (PeakAnalyser->available()) {
        _VolumePeak = PeakAnalyser->read();
        // calculate an average of the last few frames of
        // audio data -- anything greater than this AND the running beat threshold
        // will count as a beat
        if (NumFramesCounted >= AverageFrames) {
            AvgPeak = PeakRunningTotal / AverageFrames;
            PeakRunningTotal = 0;
            NumFramesCounted = 0;
        }

        PeakRunningTotal += _VolumePeak;
        NumFramesCounted++;

        bool doDetectBeat = (millis() - Timer > BeatWaitInterval);
        if (doDetectBeat && _VolumePeak > BeatCutoff && _VolumePeak > BeatMin && _VolumePeak > AvgPeak) {
            // got beat //
            NumBeats++;

            // slightly increase beat threshold //
            BeatCutoff = _VolumePeak * 1.1;

            // reset Timer //
            Timer = millis();
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
    if (RMSAnalyser->available()) {
        _VolumeRMS = RMSAnalyser->read();
    }
}

float SoundAnalyser_t::GetDominantFrequency() {
    return _DominantFreq;
}

float SoundAnalyser_t::GetVolumePeak() {
    return _VolumePeak;
}

float SoundAnalyser_t::GetVolumeRMS() {
    return _VolumeRMS;
}

uint16_t SoundAnalyser_t::BeatHappened() {
    uint16_t n = NumBeats;
    NumBeats = 0;
    return n;
}