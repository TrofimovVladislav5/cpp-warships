#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/screens/PlacementState.h>
#include <application/head/views/GameView.h>

namespace cpp_warships::head {
    /** @brief The fleet being laid out, printed as the console game used to print it:
     *  the board ruled out in ASCII, the roster under it as a list. */
    class PlainPlacementView final : public GameView {
    public:
        PlainPlacementView(const Theme& theme, MatchQuery match, const PlacementState& state);

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;

        /** @brief Reads @p stroke as it stands: printed text has nowhere to point at. */
        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    private:
        MatchQuery match_;
        const PlacementState& state_;
    };
} // namespace cpp_warships::head
