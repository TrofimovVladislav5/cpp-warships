#include <iostream>
#include <vector>
#include "Ship.h"

Ship::Ship(int length, Direction direction){
    this->length = length;
    this->direction = direction;
    segments = std::vector<SegmentState>(length, SegmentState::INT);
}

Ship::~Ship(){
    segments.clear();
}

int Ship::getLength() const {
    return length;
}

Direction Ship::getDirection() const {
    return direction;
}

std::vector<SegmentState> Ship::getSegments() const {
    return segments;
}

bool Ship::getDestroyed() const {
    for (int i = 0;i < length;i++){
        if (segments[i] != SegmentState::DESTROYED){
            return false;
        }
    }
    return true;
}

void Ship::takeDamage(const int number) {
    if (number <  0 || number >= length){
        throw std::out_of_range("Invalid length ship");
    }
    switch(segments[number]){
        case SegmentState::INT:{
            segments[number] = SegmentState::DAMAGED;
            break;
        }
        case SegmentState::DAMAGED:{
            segments[number] = SegmentState::DESTROYED;
            break;
        }
        default:
            std::cout << "C" << std::endl;
            break;
    }
}
