#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/input/EventRouter.h>
#include <application/head/intents/Intent.h>
#include <application/head/screens/PlacementState.h>
#include <application/head/screens/Screen.h>
#include <application/head/views/ViewFactory.h>

namespace cpp_warships::head {
    /** @brief The placement screen: what the player has aimed at, how it is drawn and what
     *  the keys and mouse do. It owns those three and wires them together. */
    class PlacementScreen final : public Screen {
    public:
        PlacementScreen(
                IntentSink intentSink,
                const Theme& theme,
                MatchQuery match,
                const PlacementViewFactory& makeView
        );

        [[nodiscard]] ScreenKind kind() const override;
        [[nodiscard]] GameView& view() override;
        bool handleEvent(const Keystroke& stroke) override;

    private:
        MatchQuery match_;
        PlacementState state_;
        GameViewPointer view_;
        EventRouter eventRouter_;
    };
} // namespace cpp_warships::head
