#pragma once

#include <string>

#include <ftxui/screen/color.hpp>

#include <game_core/Outcomes.h>
#include <game_tui/Theme.h>

namespace cpp_warships::game_tui {
    /** @brief How large one board cell's tile is drawn, in terminal columns and rows.
     *  An odd width is what lets a single glyph sit in the middle of its tile. */
    inline constexpr int BOARD_TILE_WIDTH = 3;
    inline constexpr int BOARD_TILE_HEIGHT = 1;

    /** @brief The breathing room left between one tile and the next, in both directions. */
    inline constexpr int BOARD_TILE_GAP = 1;

    /** @brief How far apart two neighbouring cells start. */
    inline constexpr int BOARD_COLUMN_PITCH = BOARD_TILE_WIDTH + BOARD_TILE_GAP;
    inline constexpr int BOARD_ROW_PITCH = BOARD_TILE_HEIGHT + BOARD_TILE_GAP;

    /** @brief How one board cell is drawn: a filled tile with a glyph inked on top.
     *  The tile is what makes a ship read as one body rather than a row of marks. */
    struct CellAppearance {
        std::string glyph;
        CellColors colors;
    };

    /** @brief @p glyph padded with blanks so it lands in the middle of a tile. */
    [[nodiscard]] std::string centredInTile(const std::string& glyph);

    /** @brief The appearance @p state takes under @p theme. */
    [[nodiscard]] CellAppearance appearanceOf(game_core::CellState state, const Theme& theme);

} // namespace cpp_warships::game_tui
