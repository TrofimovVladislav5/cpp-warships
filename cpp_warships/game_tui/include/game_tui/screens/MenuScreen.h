#pragma once

#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/input/EventRouter.h>
#include <game_tui/intents/Intent.h>
#include <game_tui/screens/MenuState.h>
#include <game_tui/screens/Screen.h>
#include <game_tui/views/ViewFactory.h>

namespace cpp_warships::game_tui {
    /** @brief The menu screen: what the player has chosen, how it is drawn and what the
     *  keys do. It owns those three and wires them together, holding no logic of its own. */
    class MenuScreen final : public Screen {
    public:
        MenuScreen(
                IntentSink intentSink,
                const Theme& theme,
                MatchInProgressQuery hasMatch,
                const MenuViewFactory& makeView
        );

        [[nodiscard]] ScreenKind kind() const override;
        [[nodiscard]] GameView& view() override;
        bool handleEvent(const Keystroke& stroke) override;

    private:
        MenuState state_;
        GameViewPointer view_;
        EventRouter eventRouter_;
    };
} // namespace cpp_warships::game_tui
