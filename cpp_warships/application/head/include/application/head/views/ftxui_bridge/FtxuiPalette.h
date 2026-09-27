#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

#include <application/head/Color.h>

namespace cpp_warships::head {
    /** @brief Our colour said the way FTXUI says colours. */
    [[nodiscard]] ftxui::Color toFtxuiColor(Color value);

    /** @brief The colour behind @p painted, or nothing when it is not one of ours.
     *  Only colours that went out through toFtxuiColor come back, which is every colour
     *  a theme has, because that is the one way the interface names one. */
    [[nodiscard]] std::optional<Color> colorOf(const ftxui::Color& painted);

    /** @brief Ink of @p value, so a view can go on writing colour without naming a library. */
    [[nodiscard]] ftxui::Decorator color(Color value);

    /** @brief Fill of @p value, the same way round. */
    [[nodiscard]] ftxui::Decorator bgcolor(Color value);
} // namespace cpp_warships::head
