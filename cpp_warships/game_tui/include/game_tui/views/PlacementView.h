#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>

#include <game_core/Coordinate.h>
#include <game_flow/Match.h>
#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/input/InputEvent.h>
#include <game_tui/screens/PlacementState.h>
#include <game_tui/views/BoardView.h>
#include <game_tui/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::game_tui {
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
} // namespace cpp_warships::game_tui
