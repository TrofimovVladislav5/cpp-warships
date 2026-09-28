#pragma once

#include <application/head/common/Color.h>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <optional>

namespace cpp_warships::head::tui {
    /** @brief Our colour said the way FTXUI says colours. */
    [[nodiscard]] ftxui::Color toFtxuiColor(common::Color value);

    /** @brief The colour behind @p painted, or nothing when it is not one of ours. */
    [[nodiscard]] std::optional<common::Color> colorOf(const ftxui::Color& painted);

    /** @brief Ink of @p value, so a view can go on writing colour without
     * naming a library. */
    [[nodiscard]] ftxui::Decorator color(common::Color value);

    /** @brief Fill of @p value, the same way round. */
    [[nodiscard]] ftxui::Decorator bgcolor(common::Color value);
}  // namespace cpp_warships::head::tui
