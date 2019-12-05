//
//  Delay.cpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "Delay.hpp"

typedef std::tuple<float, float> StereoPair;

//
//      Delay Line implementation
//

DelayLine::DelayLine(int sizeInSamples, float regen, float pan)
    : cpp(pan) {
        if(sizeInSamples <= 0) {
            sizeInSamples = 1;
        } // Ensure non-zero size.
    
    buffer = std::make_unique<AudioBuffer<float>>(1, sizeInSamples);
    
    buffer->clear();
    this->size = sizeInSamples;
    this->regen = regen;
}

float DelayLine::readWriteSampleMono(float sample) {
    float existing = buffer->getSample(0, tick);
    buffer->setSample(0, tick, existing*regen + sample);
    tick = (tick + 1) % size;
    return existing;
}

float DelayLine::readMono() {
    return readWriteSampleMono(0.0);
}

StereoPair DelayLine::readWriteSampleStereo(float sample) {
    return cpp.stereoPair(readWriteSampleMono(sample));
}

StereoPair DelayLine::readStereo() {
    return cpp.stereoPair(readMono());
}

float DelayLine::getPeak() {
    return buffer->getMagnitude(0, buffer->getNumSamples());
}

//
//      Delay Manager implementation
//

DelayManager::DelayManager(std::shared_ptr<RandomStore> rstore) {
    randomStore = rstore;
    activeLine = std::make_unique<DelayLine>(randomStore->getDelayTime(),
                               randomStore->getRegen(),
                               randomStore->getPan());
}

void DelayManager::newLine() {
    
    // Don't add blocks that are too quiet
    
    
    
    if(activeLine->getPeak() > silence_threshold) {
        passiveLines.push_back(std::move(activeLine));
    }
    
    auto dt = randomStore->getDelayTime()*sampleRate;
    auto r = randomStore->getRegen();
    auto p = randomStore->getPan();
    
    if (quantise) {
        dt = quantiseDelayLength(dt, quantize_subdivision); // Quantise to semiquaver
    }
    
    activeLine = std::make_unique<DelayLine>(dt,r,p);
    
    if (passiveLines.size() > max_lines) {
        passiveLines.pop_front();
    }
}

StereoPair addStereoPair(StereoPair a, StereoPair b) {
    return std::make_pair(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b));
}

StereoPair DelayManager::readWriteSampleStereo(float sample) {
    
        StereoPair sampleSum = activeLine->readWriteSampleStereo(sample);
        
        for(std::unique_ptr<DelayLine> &dl: passiveLines) {
            sampleSum = addStereoPair(sampleSum, dl->readStereo());
        }
        
        return sampleSum;
    
}

float DelayManager::readWriteSampleMono(float sample) {
    float sampleSum = activeLine->readWriteSampleMono(sample);
    for(std::unique_ptr<DelayLine> &dl: passiveLines) {
        sampleSum += dl->readMono();
    }
    return sampleSum;
}

int DelayManager::quantiseDelayLength(int unquantisedLength, int subdivision) {
    int samplesPerSubdivision = std::round((60 / tempo) * sampleRate / subdivision);
    int multiplier = std::floor(unquantisedLength / samplesPerSubdivision);
    int remainder = std::round((unquantisedLength % samplesPerSubdivision) / samplesPerSubdivision);
    int quantisedLength = multiplier*samplesPerSubdivision + remainder*samplesPerSubdivision;
    return quantisedLength;
}
