/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "QuickUI.hpp"
#include "ParameterAttachment.cpp"

//==============================================================================
/**
*/
class SndelayAudioProcessorEditor  : public AudioProcessorEditor
{
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
public:
    SndelayAudioProcessorEditor (SndelayAudioProcessor&, AudioProcessorValueTreeState& vts);
    ~SndelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SndelayAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTreeState;
    
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

    std::unique_ptr<SliderAttachment> mixAttach;
    std::unique_ptr<TwoValueSliderAttachment> delayAttach;
    
    Slider crossfadeSlider;
    Label crossfadeLabel;
    std::unique_ptr<SliderAttachment> crossfadeAttach;
    
    SingleSlider envelopes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SndelayAudioProcessorEditor)
};
