/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SndelayAudioProcessorEditor::SndelayAudioProcessorEditor (SndelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    delayTime.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
    delayTime.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    delayTime.setRange(0.1, 5);
    delayTime.setMinValue(0.5);
    delayTime.setMaxValue(1);
    delayTime.setTextValueSuffix(" s");
    delayTime.onValueChange =
        [this] {
            processor.randomStore->setDelayRange(delayTime.getMinValue(), delayTime.getMaxValue());
        };
    addAndMakeVisible(delayTime);
    
    delayTimeLabel.setText("Time", dontSendNotification);
    delayTimeLabel.attachToComponent(&delayTime, true);
    addAndMakeVisible(delayTimeLabel);
    
    regen.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
    regen.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    regen.setRange(0.1, 0.99);
    regen.setMinValue(0.5);
    regen.setMaxValue(0.85);
    regen.setTextValueSuffix(" s");
    regen.onValueChange =
    [this] {
        processor.randomStore->setRegenRange(regen.getMinValue(), regen.getMaxValue());
    };
    addAndMakeVisible(regen);
    
    regenLabel.setText("Regen", dontSendNotification);
    regenLabel.attachToComponent(&regen, true);
    addAndMakeVisible(regenLabel);
    
    pan.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
    pan.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    pan.setRange(-1, 1);
    pan.setMinValue(-1);
    pan.setMaxValue(1);
    pan.onValueChange =
    [this] {
        processor.randomStore->setPanRange(pan.getMinValue(), pan.getMaxValue());
    };
    addAndMakeVisible(pan);
    
    panLabel.setText("Pan", dontSendNotification);
    panLabel.attachToComponent(&pan, true);
    addAndMakeVisible(panLabel);
    
    mixSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    mixSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mixSlider.setRange(-1, 1);
    mixSlider.setValue(0);
    mixSlider.onValueChange =
    [this] {
        processor.mix = mixSlider.getValue();
    };
    addAndMakeVisible(mixSlider);
    
    mixLabel.setText("Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixLabel);
    
    
    quantise.onClick = [this] {
        processor.dman->quantise = quantise.getToggleState();
        
    };
    quantise.setToggleState(true, sendNotification);
    addAndMakeVisible(quantise);
    
}

SndelayAudioProcessorEditor::~SndelayAudioProcessorEditor()
{
}

//==============================================================================
void SndelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
   // g.drawFittedText ("Standard Normal Delay", getLocalBounds(), Justification::centred, 1);
}

void SndelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    int sliderLeft = 60;
    delayTime.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    regen.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    pan.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
    mixSlider.setBounds(sliderLeft, 110, getWidth() - sliderLeft - 10, 20);
    quantise.setBounds(sliderLeft, 140, getWidth() - 30, 20);
}
