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
#include <application/head/input/GridGeometry.h>
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
        /** @brief Draws into @p geometry's record of @p region, which must outlive it.
         *  Where the grid landed is written down there rather than kept here, because
         *  whatever reads the pointer is no longer the thing that drew the board. */
        BoardView(GridGeometry& geometry, ScreenRegion region) noexcept;

        [[nodiscard]] ftxui::Element render(
                const core::Board& board,
                core::Visibility visibility,
                const Theme& theme,
                const BoardOverlay& overlay
        );

    private:
        /** @brief Writes down where the grid landed when it was last painted. */
        void publishGeometry() const;

        GridGeometry& geometry_;
        ScreenRegion region_;
        ftxui::Box gridBox_;
        int boardWidth_ = 0;
        int boardHeight_ = 0;
    };
} // namespace cpp_warships::head
