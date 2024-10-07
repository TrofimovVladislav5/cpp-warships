#pragma once
#include <utility>
#include <cstddef>
enum class SegmentState{
    intact,
    damaged,
    destroyed
};

enum class Direction{
    horizontal,
    vertical
};

enum class Cell{
    empty,
    unknown,
    ship_int,
    ship_damaged,
    ship_destroyed
};

struct hashFunc {
    size_t operator()(std::pair<int, int> coordinate) const;
};