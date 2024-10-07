#pragma once
#include "Segment.h"
#include <vector>

class Ship {
private:
    int maxSegmentHealth;
    std::vector<Segment> segments;
public:
    Ship(int length, int maxSegmentHealth = 2);
    ~Ship() = default;
    int getLength();
    SegmentState getSegment(int index);
    bool takeDamage(int indexSegment, int damageCount);
    bool isDestroyed();
    void status();
};