#include "Ship.h"
#include <iostream>

Ship::Ship(int length, int maxSegmentHealth)
    : maxSegmentHealth(maxSegmentHealth)
    , segments(std::vector<Segment>(length, maxSegmentHealth))
{}

int Ship::getLength() const{
    return this->segments.size();
}

int Ship::getSegmentHitPoints(int index){
    if (index < 0 || index >= segments.size()) {
        throw std::out_of_range("Invalid index error segment");
    }
    return segments[index].getHitPoints();
}

int Ship::getMaxSegmentHealth() {
    return maxSegmentHealth;
}

bool Ship::takeDamage(int indexSegment, int damageCount) {
    if (indexSegment < 0 || indexSegment >= segments.size()) {
        throw std::out_of_range("Invalid index error");
    }

    segments[indexSegment].takeDamage(damageCount);
    return true;
}

bool Ship::isDestroyed() {
    for (auto& segment : segments) {
        if (!segment.isDestroyed()){
            return false;
        }
    }
    return true;
}