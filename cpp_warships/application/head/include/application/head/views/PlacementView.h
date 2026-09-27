#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/input/InputEvent.h>
#include <application/head/screens/PlacementState.h>
#include <application/head/views/BoardView.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::head {
    /** @brief Draws the player's board beside the fleet still waiting to be laid out.
     *  Reads the match and returns elements; it changes nothing. */
    class PlacementView final : public FtxuiView {
    public:
        PlacementView(const Theme& theme, MatchQuery match, const PlacementState& state);

        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const Theme& theme_;
        MatchQuery match_;
        const PlacementState& state_;
        BoardView boardView_;
    };
} // namespace cpp_warships::head
