//
//  ConstantPowerPanner.cpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "ConstantPowerPanner.hpp"


ConstantPowerPanner::ConstantPowerPanner(float pan) {
    leftAmp = std::abs(pan);
    rightAmp = 1 - std::abs(pan);
}

std::tuple<float, float> ConstantPowerPanner::stereoPair(float sample) {
    return std::make_pair(sample*leftAmp, sample*rightAmp);
}
