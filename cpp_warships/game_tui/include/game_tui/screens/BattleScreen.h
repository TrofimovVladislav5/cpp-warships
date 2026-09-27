#pragma once

#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/input/EventRouter.h>
#include <game_tui/intents/Intent.h>
#include <game_tui/screens/BattleState.h>
#include <game_tui/screens/Screen.h>
#include <game_tui/session/BattleJournal.h>
#include <game_tui/views/ViewFactory.h>

namespace cpp_warships::game_tui {
    /** @brief The battle screen: where the player is aiming, how it is drawn and what the
     *  keys and mouse do. It owns those three and wires them together. */
    class BattleScreen final : public Screen {
    public:
        BattleScreen(
                IntentSink intentSink,
                const Theme& theme,
                MatchQuery match,
                const BattleJournal& journal,
                const BattleViewFactory& makeView
        );

        [[nodiscard]] ScreenKind kind() const override;
        [[nodiscard]] GameView& view() override;
        bool handleEvent(const Keystroke& stroke) override;

    private:
        MatchQuery match_;
        BattleState state_;
        GameViewPointer view_;
        EventRouter eventRouter_;
    };
} // namespace cpp_warships::game_tui
