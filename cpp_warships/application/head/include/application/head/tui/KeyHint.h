#pragma once

#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace cpp_warships::head::common {
    struct Theme;
}

namespace cpp_warships::head::tui {
    /** @brief One line of the key legend: the key in a badge, what it does across from it. */
    [[nodiscard]] ftxui::Element keyHint(
        const common::Theme& theme,
        const std::string& key,
        const std::string& description
    );

    /** @brief A block of key hints, held clear of the edges of whatever
     * contains it. */
    [[nodiscard]] ftxui::Element keyLegend(std::vector<ftxui::Element> hints);
}  // namespace cpp_warships::head::tui
