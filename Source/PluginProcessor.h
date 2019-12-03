/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>
#include "Delay.hpp"
//#include "RandomStore.hpp"
#include <tuple>

typedef std::tuple<float, float> StereoPair;

//==============================================================================
/**
*/
class SndelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SndelayAudioProcessor();
    ~SndelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    bool followEnvelopes(float sample);
    
    const static int maxEnvelopes = 10;
    int numberOfEnvelopes = 9; // Add control
    std::array<float,maxEnvelopes> lastSample;
    std::array<int, maxEnvelopes> lastSign;
    std::array<Array<float>, maxEnvelopes> samples;
    Array<float> transientBuffer;
    
    float mix;
    float getDryGain();
    float getWetGain();
    
    std::shared_ptr<RandomStore> randomStore;
    std::unique_ptr<DelayManager> dman;
    
    AudioPlayHead* playHead;
    AudioPlayHead::CurrentPositionInfo currentPositionInfo;
    
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SndelayAudioProcessor)
};
