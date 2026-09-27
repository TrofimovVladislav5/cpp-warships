#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/screens/BattleState.h>
#include <application/head/session/BattleJournal.h>
#include <application/head/views/GameView.h>

namespace cpp_warships::head {
    /** @brief The battle printed the way the console game used to print it: both boards
     *  one under the other, then the skills in the bank and the story so far. */
    class PlainBattleView final : public GameView {
    public:
        PlainBattleView(
                const Theme& theme,
                MatchQuery match,
                const BattleJournal& journal,
                const BattleState& state
        );

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;

        /** @brief Reads @p stroke as it stands: printed text has nowhere to point at. */
        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    private:
        const Theme& theme_;
        MatchQuery match_;
        const BattleJournal& journal_;
        const BattleState& state_;
    };
} // namespace cpp_warships::head
