//
//  RandomStore.hpp
//  SNDelay
//
//  Created by Matt Hood on 27/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#ifndef RandomStore_hpp
#define RandomStore_hpp

#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"

#endif /* RandomStore_hpp */

class RandomStore {
public:
    RandomStore();
    float getDelayTime();
    void setDelayRange(float min, float max);
    float getRegen();
    void setRegenRange(float min, float max);
    float getPan();
    void setPanRange(float min, float max);
private:
    Range<float> delayTime;
    Range<float> regen;
    Range<float> pan;
};
