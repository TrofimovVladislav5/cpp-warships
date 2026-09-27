#pragma once

#include <optional>
#include <string>
#include <unordered_set>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <ftxui/screen/color.hpp>

#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Outcomes.h>
#include <application/head/Theme.h>
#include <application/head/views/CoordinateLabel.h>

namespace cpp_warships::head {
    /** @brief What is drawn on top of a board: where the cursor rests and which cells are marked.
     */
    struct BoardOverlay {
        std::optional<core::Coordinate> cursor;
        std::unordered_set<core::Coordinate> marked;
        CellColors markColors;
    };

    /** @brief Draws a board as a labelled grid and remembers where that grid landed.
     *  The remembered geometry is what turns a mouse position back into a cell. */
    class BoardView {
    public:
        [[nodiscard]] ftxui::Element render(
                const core::Board& board,
                core::Visibility visibility,
                const Theme& theme,
                const BoardOverlay& overlay
        );

        /** @brief The cell drawn at a screen position, or nullopt when that is off the grid. */
        [[nodiscard]] std::optional<core::Coordinate> cellAt(int screenX, int screenY) const;

    private:
        ftxui::Box gridBox_;
        int boardWidth_ = 0;
        int boardHeight_ = 0;
    };
} // namespace cpp_warships::head
