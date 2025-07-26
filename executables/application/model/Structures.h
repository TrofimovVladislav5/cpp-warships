#pragma once

#include <cstddef>
#include <utility>

namespace cpp_warships::application {
    enum class Direction { horizontal, vertical };

    enum class AttackResult { miss, damaged, destroyed, outOfBounds };

    struct hashFunc {
        size_t operator()(std::pair<int, int> coordinate) const;
    };
} // namespace cpp_warships::application
