#pragma once
#include "Structures.h"
class Segment {
private:
    int maxSegmentHealth;
    int currentSegmentHealth;
public:
    Segment(int maxSegmentHealth);
    void takeDamage(int damageCount);
    SegmentState getState() const;
    bool isDestroyed();
};