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
    float readWriteSampleMono(float sample);
    StereoPair readWriteSampleStereo(float sample);
    float readMono();
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
    StereoPair readWriteSampleStereo(float sample);
    float readWriteSampleMono(float sample);
    
    //void writeSample();
    RandomStore* randomStore;
    void  newLine();
    int   quantiseDelayLength(int unquantisedLength, int subdivision);
    float tempo = 100;
    bool  quantise = true;
    int sampleRate;
private:
    const static int max_lines = 50;
    
    std::deque<DelayLine*> passiveLines; 
    DelayLine* activeLine; // Pointer logic may get hairy here
};

