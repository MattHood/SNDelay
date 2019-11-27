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
    int getDelayTime();
    void setDelayRange(int min, int max);
    float getRegen();
    void setRegenRange(float min, float max);
    float getPan();
    void setPanRange(float min, float max);
private:
    Range<int> delayTime;
    Range<float> regen;
    Range<float> pan;
};
