#include "../include/Ship.h"

#include <iostream>

namespace cpp_warships::application {

    Ship::Ship(int length, int maxSegmentHealth) : maxSegmentHealth(maxSegmentHealth) {
        for (int i = 0; i < length; i++) {
            segments.emplace_back(new Segment(maxSegmentHealth));
        }
    }

    Ship::Ship(std::vector<Segment*> segments) : segments(segments) {}

    Ship::~Ship() {
        for (auto& segment : segments) {
            delete segment;
        }
    }

    int Ship::getLength() const { return segments.size(); }

    int Ship::getSegmentHitPoints(int index) const {
        if (index < 0 || static_cast<size_t>(index) >= segments.size()) {
            throw std::out_of_range("Invalid index error segment");
        }
        return segments[index]->getHitPoints();
    }

    int Ship::getMaxSegmentHealth() const { return maxSegmentHealth; }

    bool Ship::takeDamage(int indexSegment, int damageCount) {
        if (indexSegment < 0 || static_cast<size_t>(indexSegment) >= segments.size()) {
            throw std::out_of_range("Invalid index error");
        }

        segments[indexSegment]->takeDamage(damageCount);
        return true;
    }

    std::vector<Segment*> Ship::getSegments() const {
        return segments;
    }

    bool Ship::isDestroyed() {
        for (auto& segment : segments) {
            if (!segment->isDestroyed()) {
                return false;
            }
        }
        return true;
    }

    std::string Ship::status() {
        std::string result = "";
        for (const auto& segment : segments) {
            result += std::to_string(segment->getHitPoints()) + " ";
        }
        return result;
    }
} // namespace cpp_warships::application