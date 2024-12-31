#include "Segment.h"

Segment::Segment(int maxSegmentHealth) 
    : maxSegmentHealth(maxSegmentHealth)
    , currentSegmentHealth(maxSegmentHealth)
{}

Segment::Segment(int maxSegmentHealth, int currentSegmentHealth)
    : maxSegmentHealth(maxSegmentHealth)
    , currentSegmentHealth(currentSegmentHealth)
{}

void Segment::takeDamage(int damageCount) {
    currentSegmentHealth -= damageCount;
    if (currentSegmentHealth < 0) {
        currentSegmentHealth = 0;
    }
}
    
int Segment::getHitPoints() const {
    return currentSegmentHealth;
}

int Segment::getMaxSegmentHealth() const {
    return maxSegmentHealth;
}

bool Segment::isDestroyed() {
    return currentSegmentHealth == 0;
}

