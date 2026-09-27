#pragma once

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include <game_core/Board.h>
#include <game_core/Coordinate.h>
#include <game_core/Outcomes.h>

namespace cpp_warships::game_tui {
    /** @brief What is drawn on top of a plain board: where the cursor rests, which cells
     *  are spoken for, and the letter those cells are marked with. */
    struct PlainBoardOverlay {
        std::optional<game_core::Coordinate> cursor;
        std::unordered_set<game_core::Coordinate> marked;
        char markGlyph = '+';
    };

    /** @brief A board ruled out in ASCII, the way the console game used to draw it.
     *  Returns finished lines, so the caller only has to print them in order. */
    [[nodiscard]] std::vector<std::string> plainBoardLines(
            const game_core::Board& board,
            game_core::Visibility visibility,
            const PlainBoardOverlay& overlay
    );

    /** @brief What each cell state is written as, so a legend can be printed beside a board. */
    [[nodiscard]] std::string plainBoardLegend();
} // namespace cpp_warships::game_tui
