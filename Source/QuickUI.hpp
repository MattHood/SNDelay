//
//  QuickUI.hpp
//  SNDelay
//
//  Created by Matt Hood on 5/12/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#ifndef QuickUI_hpp
#define QuickUI_hpp

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <memory>
#include <vector>

#endif /* QuickUI_hpp */


class SingleSlider : public Component, public Slider::Listener {
public:
    
    void setProperties(String title, float min, float max, float default_value, float* data);
    SingleSlider();
    ~SingleSlider();
    void sliderValueChanged(Slider* slider) override;
    void resized() override;
   
private:
    Label title;
    Label value;
    Slider mainSlider;
    
    float* data;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SingleSlider)
    
};

class QuickUI : public Component {
public:
    QuickUI();
    ~QuickUI();
    void resized() override;
    void addSingleSlider(String title, float min, float max, float default_value, float* data);
private:
    Label quiLabel;
    std::vector<std::shared_ptr<Component>> components;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuickUI)
};
