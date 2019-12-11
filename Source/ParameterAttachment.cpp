/*
 ==============================================================================
    Copyright (c) 2019 Foleys Finest Audio Ltd. - Daniel Walz
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
    OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
    OF THE POSSIBILITY OF SUCH DAMAGE.
 ==============================================================================
 */

//#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**
 The ParameterAttachment is a class you can use to have a value synchronised with
 an AudioProcessorParameter. It takes care of updating either way, keeps the value
 thread safe in an atomic and allows setting lambdas to be executed when the value
 changes.
 */
template<typename ValueType>
class ParameterAttachment : private juce::AudioProcessorValueTreeState::Listener,
                            private juce::AsyncUpdater
{
public:
    ParameterAttachment (juce::AudioProcessorValueTreeState& stateToUse)
      : state (stateToUse)
    {
    }

    virtual ~ParameterAttachment()
    {
        detachFromParameter();
    }

    /**
     Thread safe way to read the current value.
     */
    ValueType getValue() const
    {
        return value.load();
    }

    /**
     Thread safe way to read the normalised value of the current value.
     */
    ValueType getNormalisedValue() const
    {
        if (parameter)
            return parameter->getNormalisableRange().convertTo0to1 (value.load());

        return value.load();
    }

    /**
     Set the value from a normalised 0..1 value.
     */
    void setNormalisedValue (ValueType newValue)
    {
        
        if (parameter)
            parameter->setValueNotifyingHost (newValue);
        else
            parameterChanged (paramID, juce::jlimit (0.0f, 1.0f, newValue));
    }

    /**
     Make this value attached to the parameter with the supplied parameterID.
     */
    void attachToParameter (const juce::String& parameterID)
    {
        detachFromParameter();

        paramID = parameterID;

        parameter = dynamic_cast<juce::RangedAudioParameter*>(state.getParameter (paramID));
        // Oh uh, tried to attach to a non existing parameter
        jassert (parameter != nullptr);

        initialUpdate();

        state.addParameterListener (paramID, this);
    }

    void detachFromParameter()
    {
        if (paramID.isNotEmpty())
            state.removeParameterListener (paramID, this);
    }

    /**
     Make sure to call this before you send changes (e.g. from mouseDown of your UI widget),
     otherwise the hosts automation will battle with your value changes.
     */
    void beginGesture()
    {
        if (parameter)
            parameter->beginChangeGesture();
    }

    /**
     Make sure to call this after finishing your changes (e.g. from mouseDown of your UI widget),
     this way the automation can take back control (like e.g. latch mode).
     */
    void endGesture()
    {
        if (parameter)
            parameter->endChangeGesture();
    }

    void parameterChanged (const juce::String& parameterID, float newValue) override
    {
        juce::ignoreUnused (parameterID);
        value.store (ValueType (newValue));

        if (onParameterChanged)
            onParameterChanged();

        if (onParameterChangedAsync)
            triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override
    {
        if (onParameterChangedAsync)
            onParameterChangedAsync();
    }

    /**
     Set this lambda to be called from whatever thread is updating the parameter
     */
    std::function<void()> onParameterChanged;

    /**
     Set this lambda to be called from the message thread via AsyncUpdater
     */
    std::function<void()> onParameterChangedAsync;

private:

    void initialUpdate()
    {
        value.store (ValueType (*state.getRawParameterValue (paramID)));
        if (onParameterChanged)
            onParameterChanged();
    }

    juce::AudioProcessorValueTreeState& state;
    juce::RangedAudioParameter*         parameter = nullptr;
    std::atomic<ValueType>              value;
    juce::String                        paramID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterAttachment)
};

class TwoValueSliderAttachment {
public:
    //AudioProcessorValueTreeState* valueTreeState;
    //Slider* tvSlider;
    ParameterAttachment<float> minValue;
    ParameterAttachment<float> maxValue;
    int lastDraggedThumb = 0;
    
    
    TwoValueSliderAttachment(AudioProcessorValueTreeState& vts, const String minParamID, const String maxParamID, Slider& slider)
            : minValue {vts}, maxValue {vts} {
        //valueTreeState = &vts;
        //tvSlider = &slider;
                

        // in constructor:
        slider.onDragStart = [&]
        {
            lastDraggedThumb = slider.getThumbBeingDragged();
            if (lastDraggedThumb == 1) minValue.beginGesture();
            else if (lastDraggedThumb == 2) maxValue.beginGesture();
        };
        slider.onDragEnd = [&]
        {
            if (lastDraggedThumb == 1) minValue.endGesture();
            else if (lastDraggedThumb == 2) maxValue.endGesture();
            lastDraggedThumb = 0;
        };
        
        slider.onValueChange = [&]
        {
            // Might be issues with normalisation.
            if (lastDraggedThumb == 1) minValue.setNormalisedValue(slider.getMinValue());
            else if (lastDraggedThumb == 2) maxValue.setNormalisedValue(slider.getMaxValue());
        };
        minValue.onParameterChangedAsync = [&] { slider.setMinValue (minValue.getValue()); };
        maxValue.onParameterChangedAsync = [&] { slider.setMaxValue (maxValue.getValue()); };

        minValue.attachToParameter (minParamID);
        maxValue.attachToParameter (maxParamID);
    }
};
