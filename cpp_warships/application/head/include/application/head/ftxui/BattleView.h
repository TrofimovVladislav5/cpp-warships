#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/common/PresentationContext.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/BattleState.h>
#include <application/head/ftxui/BoardView.h>
#include <application/head/ftxui/FtxuiView.h>
#include <application/model/BattleJournal.h>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>
#include <optional>

namespace cpp_warships::head {
    /** @brief Draws both fleets, the skills in the bank and the story so far.
     *  Reads the match and returns elements; it changes nothing. */
    class BattleView final : public FtxuiRenderer {
       public:
        BattleView(const PresentationContext& context, GridGeometry& geometry) noexcept;

       protected:
        [[nodiscard]] ftxui::Element renderElement() override;

       private:
        /** @brief Writes down where the log landed when it was last painted. */
        void publishLogGeometry() const;

        /** @brief Whether a screen position falls inside the log, which scrolls
         * on its own. */

        const PresentationContext& context_;
        BoardView ownWatersView_;
        BoardView enemyWatersView_;
        GridGeometry& geometry_;
        ftxui::Box logBox_;
    };
}  // namespace cpp_warships::head
