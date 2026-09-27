#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>

#include <game_core/Coordinate.h>
#include <game_flow/Match.h>
#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/input/InputEvent.h>
#include <game_tui/screens/BattleState.h>
#include <game_tui/session/BattleJournal.h>
#include <game_tui/views/BoardView.h>
#include <game_tui/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::game_tui {
    /** @brief Draws both fleets, the skills in the bank and the story so far.
     *  Reads the match and returns elements; it changes nothing. */
    class BattleView final : public FtxuiView {
    public:
        BattleView(
                const Theme& theme,
                MatchQuery match,
                const BattleJournal& journal,
                const BattleState& state
        );

        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        /** @brief Whether a screen position falls inside the log, which scrolls on its own. */
        [[nodiscard]] bool isOverLog(int screenX, int screenY) const;

        const Theme& theme_;
        MatchQuery match_;
        const BattleJournal& journal_;
        const BattleState& state_;
        BoardView ownWatersView_;
        BoardView enemyWatersView_;
        ftxui::Box logBox_;
    };
} // namespace cpp_warships::game_tui
