//
//  QuickUI.cpp
//  SNDelay
//
//  Created by Matt Hood on 5/12/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "QuickUI.hpp"

SingleSlider::SingleSlider() {
    
}

void SingleSlider::setProperties(String mtitle, float min, float max, float default_value, float* mdata) {
    
    data = mdata;
    
    mainSlider.setSliderStyle(Slider::LinearHorizontal);
    mainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mainSlider.setRange(min, max);
    mainSlider.setValue(default_value);
    mainSlider.addListener(this);
    addAndMakeVisible(mainSlider);
    
    value.setText(String(mainSlider.getValue()), dontSendNotification);
    value.attachToComponent(&mainSlider, true);
    addAndMakeVisible(value);
    
    title.setText(mtitle, dontSendNotification);
    title.setFont(Font(12, italic));
    title.attachToComponent(&value, true);
    addAndMakeVisible(title);
    
}
SingleSlider::~SingleSlider() {
    
}

void SingleSlider::sliderValueChanged(Slider* slider)  {
    *data = mainSlider.getValue();
    value.setText(String((int)mainSlider.getValue()), dontSendNotification);
    
    
}

void SingleSlider::resized() {
    auto titleWidth = title.getWidth();
    auto valueWidth = value.getWidth();
    mainSlider.setBounds(titleWidth + valueWidth + 100, 0, 300, 50);
}










QuickUI::QuickUI() {
    //setSize(200,200);
    quiLabel.setText("QUI Test", dontSendNotification);
    addAndMakeVisible(quiLabel);
}

QuickUI::~QuickUI() {
    
}
void QuickUI::resized() {
    quiLabel.setBounds(10, 10, 50, 30);
    int y_index = 30;
    for(std::shared_ptr<Component> comp: components) {
        int height = comp->getHeight() + 10;
        comp->setBounds(0, y_index, 400, 40);
        y_index += 20 + height;
    }
    
}
void QuickUI::addSingleSlider(String title, float min, float max, float default_value, float* data) {
    std::shared_ptr<SingleSlider> single_slider = std::make_shared<SingleSlider>();
    single_slider->setProperties(title, min, max, default_value, data);
    components.push_back(std::move(single_slider));
    addAndMakeVisible(*components.back());
    resized();
}

