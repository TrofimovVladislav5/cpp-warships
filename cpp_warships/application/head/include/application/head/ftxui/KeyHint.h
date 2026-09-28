#pragma once

#include <application/head/common/Theme.h>

#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace cpp_warships::head {
    /** @brief One line of the key legend: the key in a badge, what it does
     * across from it. The two are pushed apart, so every meaning lines up down
     * the right of the block. */
    [[nodiscard]] ftxui::Element
    keyHint(const Theme& theme, const std::string& key, const std::string& description);

    /** @brief A block of key hints, held clear of the edges of whatever
     * contains it. */
    [[nodiscard]] ftxui::Element keyLegend(std::vector<ftxui::Element> hints);
}  // namespace cpp_warships::head
