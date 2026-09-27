#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/input/EventRouter.h>
#include <application/head/intents/Intent.h>
#include <application/head/screens/MenuState.h>
#include <application/head/screens/Screen.h>
#include <application/head/views/ViewFactory.h>

namespace cpp_warships::head {
    /** @brief The menu screen: what the player has chosen, how it is drawn and what the
     *  keys do. It owns those three and wires them together, holding no logic of its own. */
    class MenuScreen final : public Screen {
    public:
        MenuScreen(
                IntentSink intentSink,
                const Theme& theme,
                MatchInProgressQuery hasMatch,
                SavedMatchQuery hasSavedMatch,
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
} // namespace cpp_warships::head
