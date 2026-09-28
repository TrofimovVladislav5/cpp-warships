#pragma once

#include <application/head/common/Color.h>

#include <string>
#include <vector>

namespace cpp_warships::head {
    /** @brief A filled board cell: the colour of the tile and the ink of the
     * glyph on it. Board cells are drawn as rectangles, so every tile colour
     * needs a legible partner. */
    struct CellColors {
        Color fill;
        Color ink;
    };

    /** @brief Every colour the interface may use, named by role rather than by
     * hue. Screens read these slots and never name a colour themselves. */
    struct Theme {
        std::string name;

        Color background;
        Color surface;
        Color border;
        Color text;
        Color textMuted;
        Color accent;

        CellColors water;
        CellColors ship;
        CellColors damaged;
        CellColors destroyed;
        CellColors sunk;
        CellColors miss;
        CellColors cursor;

        Color danger;
        Color success;
    };

    /** @brief The themes the interface can be dressed in, in the order they are
     * offered. */
    [[nodiscard]] const std::vector<Theme>& availableThemes();

    /** @brief The theme used until the player picks another. */
    [[nodiscard]] const Theme& defaultTheme();

    /** @brief The theme called @p name, or the default when there is no such
     * theme.
     */
    [[nodiscard]] const Theme& themeNamed(const std::string& name);
}  // namespace cpp_warships::head
