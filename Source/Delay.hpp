//
//  Delay.hpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#ifndef Delay_hpp
#define Delay_hpp

#include <stdio.h>
#include <deque>
#include "../JuceLibraryCode/JuceHeader.h"
#include "ConstantPowerPanner.hpp"
#include "RandomStore.hpp"
#include <memory>

#endif /* Delay_hpp */

typedef std::tuple<float, float> StereoPair;
class DelayLine {
public:
    DelayLine(int sizeInSamples, float regen, float pan = 0);
    float readMono();
    StereoPair readStereo();
    float getPeak();
    void write(float sample);
    void increment();
    
private:
    float regen;
    int tick = 0;
    int size;
    float sample_copy;
    bool didExplicitlyWrite = false;
    ConstantPowerPanner cpp;
    AudioBuffer<float> buffer;
    StereoPair stereoPair(float sample, float angle);
};

class DelayManager : public AudioProcessorValueTreeState::Listener {
public:
    DelayManager(std::shared_ptr<RandomStore> mRandomStore);
    StereoPair readWriteSampleStereo(float sample);
    float readWriteSampleMono(float sample);
    void parameterChanged(const String & id, float newValue) override;
    
    //void writeSample();
    std::shared_ptr<RandomStore> randomStore;
    void  newLine();
    int   quantiseDelayLength(int unquantisedLength, int subdivision);
    
    float silence_threshold = 0.0001; // Add control
    int sampleRate = 44100;
    float tempo = 100; // Add control
    int quantise_subdivision = 4;
    bool  quantise = true;
    int crossfade_samples = 10;
    float input_crossfade_samples = (float) crossfade_samples;
    bool inTransition = false;
    
private:
    void doCrossfadeWriteIncrement(float inputSample);
    
    const static int max_lines = 50;
    int crossfade_counter = 0;
    
    
    std::deque<std::unique_ptr<DelayLine>> passiveLines;
    std::unique_ptr<DelayLine> activeLine; // Pointer logic may get hairy here
    std::unique_ptr<DelayLine> incomingLine;
    std::unique_ptr<DelayLine> outgoingLine;
};

