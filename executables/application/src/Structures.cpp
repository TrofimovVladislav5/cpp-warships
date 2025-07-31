#include "../include/Structures.h"

#include <cstddef>

namespace cpp_warships::application {

    size_t hashFunc::operator()(std::pair<int, int> coordinate) const {
        return coordinate.first + coordinate.second;
    }
} // namespace cpp_warships::application