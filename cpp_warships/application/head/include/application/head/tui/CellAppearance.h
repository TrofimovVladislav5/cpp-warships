#pragma once

#include <application/core/Outcomes.h>
#include <application/head/common/Theme.h>

#include <ftxui/screen/color.hpp>
#include <string>

namespace cpp_warships::head::common {
    struct Theme;
}

namespace cpp_warships::head::tui {
    /** @brief How large one board cell's tile is drawn, in terminal columns and rows. */
    inline constexpr int BOARD_TILE_WIDTH = 3;
    inline constexpr int BOARD_TILE_HEIGHT = 1;

    /** @brief The breathing room left between one tile and the next, in both
     * directions. */
    inline constexpr int BOARD_TILE_GAP = 1;

    /** @brief How far apart two neighbouring cells start. */
    inline constexpr int BOARD_COLUMN_PITCH = BOARD_TILE_WIDTH + BOARD_TILE_GAP;
    inline constexpr int BOARD_ROW_PITCH = BOARD_TILE_HEIGHT + BOARD_TILE_GAP;

    /** @brief How one board cell is drawn: a filled tile with a glyph inked on top. */
    struct CellAppearance {
        std::string glyph;
        common::CellColors colors;
    };

    /** @brief @p glyph padded with blanks so it lands in the middle of a tile. */
    [[nodiscard]] std::string centredInTile(const std::string& glyph);

    /** @brief The appearance @p state takes under @p theme. */
    [[nodiscard]] CellAppearance appearanceOf(core::CellState state, const common::Theme& theme);
}  // namespace cpp_warships::head::tui
