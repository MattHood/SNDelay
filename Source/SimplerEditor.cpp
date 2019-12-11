/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "SimplerEditor.h"

void SndelayAudioProcessorEditor::sliderBuilder(Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attach, String param, String title) {
    slider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 70, 24);
    addAndMakeVisible(slider);
    attach.reset(new SliderAttachment(valueTreeState, param, slider));
    label.setText(title, dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
}

//==============================================================================
SndelayAudioProcessorEditor::SndelayAudioProcessorEditor (SndelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 600);
    
//    delayMin.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
//    delayMin.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 70, 24);
//    addAndMakeVisible(delayMin);
//    delayMinAttach.reset(new SliderAttachment(vts, "delay_min", delayMin));
//    delayMinLabel.setText("Delay Min", dontSendNotification);
//    delayMinLabel.attachToComponent(&delayMin, true);
//    addAndMakeVisible(delayMinLabel);
//
//    delayMax.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
//    delayMax.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 70, 24);
//    addAndMakeVisible(delayMax);
//    delayMaxAttach.reset(new SliderAttachment(vts, "delay_max", delayMax));
//    delayMinLabel.setText("Delay Max", dontSendNotification);
//    delayMaxLabel.attachToComponent(&delayMax, true);
//    addAndMakeVisible(delayMaxLabel);
   
    sliderBuilder(delayMin, delayMinLabel, delayMinAttach, "delay_min", "Delay Min");
    sliderBuilder(delayMax, delayMaxLabel, delayMaxAttach, "delay_max", "Delay Max");
    sliderBuilder(regenMin, regenMinLabel, regenMinAttach, "regen_min", "Regen Min");
    sliderBuilder(regenMax, regenMaxLabel, regenMaxAttach, "regen_max", "Regen Max");
    sliderBuilder(panMin, panMinLabel, panMinAttach, "pan_min", "Pan Min");
    sliderBuilder(panMax, panMaxLabel, panMaxAttach, "pan_max", "Pan Max");
    sliderBuilder(mixSlider, mixLabel, mixAttach, "mix", "Mix");
    
    quantiseButton.setToggleState(true, sendNotification);
    addAndMakeVisible(quantiseButton);
    quantiseAttach.reset(new ButtonAttachment(valueTreeState, "quantise",quantiseButton));
    
    sliderBuilder(envelopesSlider, envelopesLabel, envelopesAttach, "envelopes", "Envelopes");
    sliderBuilder(crossfadeSlider, crossfadeLabel, crossfadeAttach, "crossfade", "Crossfade");
    
    
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
    
    auto area = getLocalBounds();
    int slider_height = 25;
    area.removeFromLeft(75);
    delayMin.setBounds(area.removeFromTop(slider_height));
    delayMax.setBounds(area.removeFromTop(slider_height));
    regenMin.setBounds(area.removeFromTop(slider_height));
    regenMax.setBounds(area.removeFromTop(slider_height));
    panMin.setBounds(area.removeFromTop(slider_height));
    panMax.setBounds(area.removeFromTop(slider_height));
    mixSlider.setBounds(area.removeFromTop(slider_height));
    quantiseButton.setBounds(area.removeFromTop(slider_height));
    envelopesSlider.setBounds(area.removeFromTop(slider_height));
    crossfadeSlider.setBounds(area.removeFromTop(slider_height));
    
    
    
    
}
