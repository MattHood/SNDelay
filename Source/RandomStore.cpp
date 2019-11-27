//
//  RandomStore.cpp
//  SNDelay
//
//  Created by Matt Hood on 27/11/19.
//  Copyright © 2019 Swoop Maggie. All rights reserved.
//

#include "RandomStore.hpp"

RandomStore::RandomStore() {
    // Reasonable defaults
    delayTime.setStart(10000);
    delayTime.setEnd(40000);
    regen.setStart(0.5);
    regen.setEnd(0.85);
    pan.setStart(-1);
    pan.setEnd(1);
}

int RandomStore::getDelayTime() {
    return Random::getSystemRandom().nextInt(delayTime);
}
void RandomStore::setDelayRange(int min, int max) {
    delayTime.setStart(min);
    delayTime.setEnd(max);
}
float RandomStore::getRegen() {
    float multiplier = regen.getLength();
    float offset = regen.getStart();
    return Random::getSystemRandom().nextFloat()*multiplier + offset;
}
void RandomStore::setRegenRange(float min, float max) {
    regen.setStart(min);
    regen.setEnd(max);
}
float RandomStore::getPan() {
    float multiplier = pan.getLength();
    float offset = pan.getStart();
    return Random::getSystemRandom().nextFloat()*multiplier + offset;
}
void RandomStore::setPanRange(float min, float max) {
    regen.setStart(min);
    regen.setEnd(max);
}