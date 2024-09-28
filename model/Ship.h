#pragma once

#include <vector>

#include "Ship.h"
#include "Structures.h"

class Ship {
private:
    Direction direction;
    int length;
    std::vector<SegmentState> segments;
public:
    Ship(int length, Direction direction);
    ~Ship();

    int getLength() const;
    Direction getDirection() const;
    bool getDestroyed() const;
    void takeDamage(int number);
};