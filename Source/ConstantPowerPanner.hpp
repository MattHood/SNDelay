//
//  ConstantPowerPanner.hpp
//  SNDelay
//
//  Created by Matt Hood on 26/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#ifndef ConstantPowerPanner_hpp
#define ConstantPowerPanner_hpp

#include <stdio.h>
#include <tuple>
#include <cmath>
#include <iostream>

#endif /* ConstantPowerPanner_hpp */

typedef std::tuple<float, float> StereoPair;

class ConstantPowerPanner {
public:
    ConstantPowerPanner(float pan);
    StereoPair stereoPair(float sample);
    
private:
    float leftAmp;
    float rightAmp;
};
