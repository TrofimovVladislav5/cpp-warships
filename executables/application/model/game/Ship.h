#pragma once
#include <string>
#include <vector>

#include "Segment.h"

class Ship {
private:
    int maxSegmentHealth;
    std::vector<Segment*> segments;

public:
    explicit Ship(int length, int maxSegmentHealth = 2);
    explicit Ship(std::vector<Segment*> segments);
    ~Ship();
    [[nodiscard]] int getLength() const;
    [[nodiscard]] int getSegmentHitPoints(int index) const;
    [[nodiscard]] int getMaxSegmentHealth() const;
    [[nodiscard]] std::vector<Segment*> getSegments() const;
    bool takeDamage(int indexSegment, int damageCount);
    bool isDestroyed();
    std::string status();
};
