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

DelayLine::DelayLine(int sizeInSamples, float regen, float pan, float lpFrequency, float hpFrequency, float sampleRate)
    : cpp(pan),
      buffer(1, sizeInSamples == 0 ? 1 : sizeInSamples) {
        
        if(sizeInSamples <= 0) {
            sizeInSamples = 1;
        } // Ensure non-zero size.
    
    
    
          buffer.clear();
    
          this->size = sizeInSamples;
          this->regen = regen;
          
          lowPass.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, lpFrequency));
          highPass.setCoefficients(IIRCoefficients::makeHighPass(sampleRate, hpFrequency));
}

float DelayLine::readMono() {
    sample_copy = buffer.getSample(0, tick);
    return sample_copy;
}

StereoPair DelayLine::readStereo() {
    return cpp.stereoPair(readMono());
}

void DelayLine::write(float sample) {
    float preFilter = sample_copy*regen + sample;
    float postFilter = highPass.processSingleSampleRaw(lowPass.processSingleSampleRaw(preFilter));
    buffer.setSample(0, tick, postFilter);
    didExplicitlyWrite = true;
}

void DelayLine::increment() {
    
    if (!didExplicitlyWrite) {
        write(0);
    }
    tick = (tick + 1) % size;
    didExplicitlyWrite = false;
}

float DelayLine::getPeak() {
    return buffer.getMagnitude(0, buffer.getNumSamples());
}

//
//      Delay Manager implementation
//

DelayManager::DelayManager(std::shared_ptr<RandomStore> rstore) {
    randomStore = rstore;
    auto dt = randomStore->getDelayTime();
    activeLine = std::make_unique<DelayLine>(dt,
                               randomStore->getRegen(),
                               randomStore->getPan());
    
}

void DelayManager::newLine() {
    // I think this is the best place to do this, saves moving the goalposts halfway through the crossfade. When it is replaced by a proper slider, make sure that there is appropriate caching of the value
    inTransition = true;
    //crossfade_samples = (int)input_crossfade_samples;
    jassert(activeLine != NULL);
    outgoingLine = std::move(activeLine);
    
    auto dt = randomStore->getDelayTime()*sampleRate;
    auto r = randomStore->getRegen();
    auto p = randomStore->getPan();
    
    if (quantise) {
        dt = quantiseDelayLength(dt, quantise_subdivision); // Quantise to semiquaver
    }
    
    incomingLine = std::make_unique<DelayLine>(dt,r,p,lpFrequency,hpFrequency,sampleRate);
    jassert(incomingLine != NULL);
}

void DelayManager::doCrossfadeWriteIncrement(float inputSample) {
    
    
    if (crossfade_counter < crossfade_samples) {
        float ascendingGain = (float)crossfade_counter / (float)crossfade_samples; // 0!
        
        float descendingGain = 1 - ascendingGain;

        incomingLine->write(inputSample*ascendingGain);
        outgoingLine->write(inputSample*descendingGain);
        
        incomingLine->increment();
        outgoingLine->increment();
        
        crossfade_counter++;
    }
    
    if (crossfade_counter == crossfade_samples) {
        activeLine = std::move(incomingLine);
        jassert(activeLine != NULL);
        if (passiveLines.size() > max_lines) {
            passiveLines.pop_front();
        }
        
        if (outgoingLine->getPeak() > silence_threshold) {
            passiveLines.push_back(std::move(outgoingLine));
        }
        crossfade_counter = 0;
        inTransition = false;
    }
    
    
    
}

StereoPair addStereoPair(StereoPair a, StereoPair b) {
    return std::make_pair(std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b));
}

StereoPair DelayManager::readWriteSampleStereo(float sample) {
    StereoPair sampleSum;
    jassert((activeLine != NULL) != inTransition);
    if (inTransition) {
        
        sampleSum = addStereoPair(incomingLine->readStereo(), outgoingLine->readStereo());
        doCrossfadeWriteIncrement(sample);
    }
    else {
        sampleSum = activeLine->readStereo();
        activeLine->write(sample);
        activeLine->increment();
    }
        
    for(std::unique_ptr<DelayLine> &dl: passiveLines) {
        sampleSum = addStereoPair(sampleSum, dl->readStereo());
        dl->increment();
    }
        
    return sampleSum;
    
}

float DelayManager::readWriteSampleMono(float sample) {
    float sampleSum;
    
    jassert((activeLine != NULL) != inTransition);
    if (inTransition) {
        
        sampleSum = incomingLine->readMono() + outgoingLine->readMono();
        doCrossfadeWriteIncrement(sample);
    }
    else {
        sampleSum = activeLine->readMono();
        activeLine->write(sample);
        activeLine->increment();
    }
    
    for(std::unique_ptr<DelayLine> &dl: passiveLines) {
        sampleSum += dl->readMono();
        dl->increment();
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

void DelayManager::parameterChanged(const String & id, float newValue) {
    
    if (id == "silence_threshold") {
        silence_threshold = newValue;
    }
    else if (id == "sample_rate") {
        sampleRate = (int)newValue;
    }
    else if (id == "tempo") {
        tempo = newValue;
    }
    else if (id == "subdivision") {
        quantise_subdivision = newValue;
    }
    else if (id == "quantise") {
        quantise = (bool)newValue;
    }
    else if (id == "crossfade") {
        crossfade_samples = (int)newValue;
    }
    else if (id == "hp_frequency") {
        hpFrequency = newValue;
    }
    else if (id == "lp_frequency") {
        lpFrequency = newValue;
    }
    
}
