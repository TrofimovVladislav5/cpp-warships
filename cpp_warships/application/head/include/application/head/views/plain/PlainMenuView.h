#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/screens/MenuState.h>
#include <application/head/views/GameView.h>

namespace cpp_warships::head {
    /** @brief The menu as the console game used to print it: a few lines, no colour.
     *  Reads what it was given and returns text; it changes nothing. */
    class PlainMenuView final : public GameView {
    public:
        PlainMenuView(
                const Theme& theme,
                const MenuState& state,
                MatchInProgressQuery hasMatch,
                SavedMatchQuery hasSavedMatch
        );

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;

        /** @brief Reads @p stroke as it stands: printed text has nowhere to point at. */
        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    private:
        const Theme& theme_;
        const MenuState& state_;
        MatchInProgressQuery hasMatch_;
        SavedMatchQuery hasSavedMatch_;
    };
} // namespace cpp_warships::head
