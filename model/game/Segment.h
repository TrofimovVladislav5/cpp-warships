#pragma once

class Segment {
private:
    int maxSegmentHealth;
    int currentSegmentHealth;
public:
    Segment(int maxSegmentHealth);
    Segment(int maxSegmentHealth, int currentSegmentHealth);
    void takeDamage(int damageCount);
    [[nodiscard]] int getHitPoints() const;
    [[nodiscard]] int getMaxSegmentHealth() const;
    bool isDestroyed();
};