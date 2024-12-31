#pragma once
#include <utility>
#include <cstddef>

enum class Direction{
    horizontal,
    vertical
};

enum class AttackResult{
    miss,
    damaged,
    destroyed,
    outOfBounds
};

struct hashFunc {
    size_t operator()(std::pair<int, int> coordinate) const;
};