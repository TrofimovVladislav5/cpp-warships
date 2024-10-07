#include "Segment.h"

Segment::Segment(int maxSegmentHealth) 
    : maxSegmentHealth(maxSegmentHealth)
    , currentSegmentHealth(maxSegmentHealth)
{}

void Segment::takeDamage(int damageCount) {
    currentSegmentHealth -= damageCount;
    if (currentSegmentHealth < 0) {
        currentSegmentHealth = 0;
    }
}
    
SegmentState Segment::getState() const {
    if (currentSegmentHealth == maxSegmentHealth) {
        return SegmentState::intact;
    }
    else if (currentSegmentHealth > 0) {
        return SegmentState::damaged;
    }
    else {
        return SegmentState::destroyed;
    }
}

bool Segment::isDestroyed() {
    return currentSegmentHealth == 0;
}