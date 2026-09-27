#pragma once

#include <string>

#include <game_core/Coordinate.h>

namespace cpp_warships::game_tui {
    /** @brief The letter a board column is headed with, or "?" past the end of the alphabet. */
    [[nodiscard]] std::string columnLabel(int column);

    /** @brief How a cell is written down, the way the grid labels it: column letter, row number. */
    [[nodiscard]] std::string coordinateLabel(game_core::Coordinate coordinate);
} // namespace cpp_warships::game_tui
