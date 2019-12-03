/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SndelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SndelayAudioProcessorEditor (SndelayAudioProcessor&);
    ~SndelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SndelayAudioProcessor& processor;
    Slider delayTime;
    Slider regen;
    Slider pan;
    Slider mixSlider;
    Slider filterSlider;
    
    Label delayTimeLabel;
    Label regenLabel;
    Label panLabel;
    Label mixLabel;
    Label filterLabel;
    
    ToggleButton quantise{"Quantise"};
    Label tempoLabel;
    Label bpmLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SndelayAudioProcessorEditor)
};
