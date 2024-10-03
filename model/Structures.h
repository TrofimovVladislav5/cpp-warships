#pragma once

enum class Direction{
    HORIZONTAL,
    VERTICAL
};

enum class SegmentState{
    INT,
    DAMAGED,
    DESTROYED
};

enum class CellField {
    UNKNOWN,
    EMPTY,
    SHIP,
    SHIP_INT,
    SHIP_DAMAGED,
    SHIP_DESTROYED
};