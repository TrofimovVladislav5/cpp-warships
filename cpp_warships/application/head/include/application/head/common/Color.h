#pragma once

#include <cstdint>

namespace cpp_warships::head {
    /** @brief A colour as plain channels, so a palette owes nothing to any
     * drawing library. Whatever finally paints it is what knows how to say this
     * to a terminal. */
    struct Color {
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;
    };

    [[nodiscard]] constexpr bool operator==(const Color left, const Color right) {
        return left.red == right.red && left.green == right.green && left.blue == right.blue;
    }

    [[nodiscard]] constexpr bool operator!=(const Color left, const Color right) {
        return !(left == right);
    }
}  // namespace cpp_warships::head
