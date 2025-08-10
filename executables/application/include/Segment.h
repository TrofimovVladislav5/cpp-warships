#pragma once

#include <cpp_warships/game_saves/include/ISerializable.h>

namespace cpp_warships::application {
    inline char SegmentName[] = "Segment";

    class Segment
        : public game_saves::ISerializable<SegmentName>
    {
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
} // namespace cpp_warships::application
