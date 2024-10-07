#include "Ship.h"
#include <iostream>

Ship::Ship(int length, int maxSegmentHealth = 2) 
    : maxSegmentHealth(maxSegmentHealth)
    , segments(std::vector<Segment>(length, maxSegmentHealth))
{}

Ship::~Ship() = default;

int Ship::getLength() {return segments.size();}

SegmentState Ship::getSegment(int index){
    if (index < 0 || index >= segments.size()) {
        throw std::out_of_range("Invalid index error segment");
    }
    return segments[index].getState();
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

void Ship::status(){
    std::string shipInfo;
    for (int i = segments.size() - 1;i >= 0;i--) {
        switch (segments[i].getState()){
            case SegmentState::intact:
                shipInfo += " int ";
                break;
            case SegmentState::damaged:
                shipInfo += " damaged ";
                break;
            case SegmentState::destroyed:
                shipInfo += " destroyed ";
                break;
            }
        }
    std::cout << "Segments info: " << shipInfo << std::endl;
}