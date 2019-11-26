//
//  ConstantPowerPanner.cpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "ConstantPowerPanner.hpp"



ConstantPowerPanner::ConstantPowerPanner(float pan) {
    std::cout << "Pam: " << pan << std::endl;
    const float pi = 3.1415926545;
    float angle_adjusted = (pan + 1)*(pi/4);
    
    leftAmp = std::cos(angle_adjusted);
    rightAmp = std::sin(angle_adjusted);
}

std::tuple<float, float> ConstantPowerPanner::stereoPair(float sample) {
    return std::make_pair(sample*leftAmp, sample*rightAmp);
}
