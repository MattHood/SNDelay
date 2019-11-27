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

#endif /* Delay_hpp */

typedef std::tuple<float, float> StereoPair;
class DelayLine {
public:
    DelayLine(int sizeInSamples, float regen, float pan = 0);
    float readWriteSample(float sample);
    StereoPair readWriteStereo(float sample);
    float readOnly();
    StereoPair readStereo();
    
private:
    float regen;
    int tick;
    int size;
    ConstantPowerPanner cpp;
    std::vector<float> buffer;
    StereoPair stereoPair(float sample, float angle);
};

class DelayManager {
public:
    DelayManager(RandomStore* mRandomStore);
    StereoPair readWriteSample(float sample);
    
    //void writeSample();
    RandomStore* randomStore;
    void newLine();
    int quantiseDelayLength(int unquantisedLength, float tempo, int sampleRate, int subdivision);
private:
    const static int max_lines = 50;
    std::deque<DelayLine*> passiveLines; 
    DelayLine* activeLine; // Pointer logic may get hairy here
};

