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
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
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
    
    void sliderBuilder(Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attach, String param, String title);
    
    Slider delayMin;
    Label delayMinLabel;
    std::unique_ptr<SliderAttachment> delayMinAttach;
    
    Slider delayMax;
    Label delayMaxLabel;
    std::unique_ptr<SliderAttachment> delayMaxAttach;
    
    Slider regenMin;
    Label regenMinLabel;
    std::unique_ptr<SliderAttachment> regenMinAttach;
    
    Slider regenMax;
    Label regenMaxLabel;
    std::unique_ptr<SliderAttachment> regenMaxAttach;
    
    Slider panMin;
    Label panMinLabel;
    std::unique_ptr<SliderAttachment> panMinAttach;
    
    Slider panMax;
    Label panMaxLabel;
    std::unique_ptr<SliderAttachment> panMaxAttach;

    Slider mixSlider;
    Label mixLabel;
    std::unique_ptr<SliderAttachment> mixAttach;
    
    ToggleButton quantiseButton;
    Label quantiseLabel;
    std::unique_ptr<ButtonAttachment> quantiseAttach;
    
    Slider tempoSlider;
    Label tempoLabel;
    std::unique_ptr<SliderAttachment> tempoAttach;
    
    Slider crossfadeSlider;
    Label crossfadeLabel;
    std::unique_ptr<SliderAttachment> crossfadeAttach;
    
    Slider envelopesSlider;
    Label envelopesLabel;
    std::unique_ptr<SliderAttachment> envelopesAttach;
    
    Slider highpassSlider;
    Label highpassLabel;
    std::unique_ptr<SliderAttachment> highpassAttach;
    
    Slider lowpassSlider;
    Label lowpassLabel;
    std::unique_ptr<SliderAttachment> lowpassAttach;
    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SndelayAudioProcessorEditor)
};
