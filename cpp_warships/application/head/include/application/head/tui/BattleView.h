#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/BattleState.h>
#include <application/head/tui/BoardView.h>
#include <application/head/tui/FtxuiView.h>
#include <application/model/BattleJournal.h>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <optional>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::tui {
    /** @brief Draws both fleets, the skills in the bank and the story so far.
     * Reads the match and returns elements; it changes nothing. */
    class BattleView final : public FtxuiRenderer {
    public:
        BattleView(
            const common::PresentationContext& context,
            common::input::GridGeometry& geometry
        ) noexcept;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        /** @brief Writes down where the log landed when it was last painted. */
        void publishLogGeometry() const;

        /** @brief Whether a screen position falls inside the log, which scrolls
         * on its own. */

        const common::PresentationContext& context_;
        BoardView ownWatersView_;
        BoardView enemyWatersView_;
        common::input::GridGeometry& geometry_;
        ftxui::Box logBox_;
    };
}  // namespace cpp_warships::head::tui
