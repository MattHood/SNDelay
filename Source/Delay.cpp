//
//  Delay.cpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "Delay.hpp"

typedef std::tuple<float, float> StereoPair;

DelayLine::DelayLine(int sizeInSamples, float regen, float pan)
    : cpp(pan) {
    buffer.resize(sizeInSamples);
    this->size = sizeInSamples;
    this->regen = regen;
    
}

float DelayLine::readWriteSample(float sample) {
    float existing = buffer[tick];
    buffer[tick] = existing*regen + sample;
    tick = (tick + 1) % size;
    return existing;
}

float DelayLine::readOnly() {
    return readWriteSample(0.0);
}

StereoPair DelayLine::readWriteStereo(float sample) {
    return cpp.stereoPair(readWriteSample(sample));
}

StereoPair DelayLine::readStereo() {
    return cpp.stereoPair(readOnly());
}

void DelayManager::newLine() {
    passiveLines.push_back(activeLine);
    int delayTime = Random::getSystemRandom().nextInt(44100) + 10000;
    float pan = Random::getSystemRandom().nextFloat()*2 - 1; // -1 <= pan <= 1
    activeLine = new DelayLine(delayTime, 0.5, pan);
    
    if (passiveLines.size() > 20) {
        delete passiveLines.front();
        passiveLines.pop_front(); // Memory leak?
        
    }
}

StereoPair addStereoPair(StereoPair a, StereoPair b) {
    return std::make_pair(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b));
}

DelayManager::DelayManager() {
    int delayTime = Random::getSystemRandom().nextInt(35000) + 10000;
    float pan = Random::getSystemRandom().nextFloat()*2 - 2;
    activeLine = new DelayLine(delayTime, 0.9, pan);
}

StereoPair DelayManager::readWriteSample(float sample) {
    StereoPair activeSample = activeLine->readWriteStereo(sample);
    StereoPair sampleSum = activeSample;
    
    for(DelayLine* dl: passiveLines) {
        sampleSum = addStereoPair(sampleSum, dl->readStereo());
    }
    
    return sampleSum;
}
