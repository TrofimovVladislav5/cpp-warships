#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/input/EventRouter.h>
#include <application/head/intents/Intent.h>
#include <application/head/screens/BattleState.h>
#include <application/head/screens/Screen.h>
#include <application/model/BattleJournal.h>
#include <application/head/views/ViewFactory.h>

namespace cpp_warships::head {
    /** @brief The battle screen: where the player is aiming, how it is drawn and what the
     *  keys and mouse do. It owns those three and wires them together. */
    class BattleScreen final : public Screen {
    public:
        BattleScreen(
                IntentSink intentSink,
                const Theme& theme,
                MatchQuery match,
                const model::BattleJournal& journal,
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
} // namespace cpp_warships::head
