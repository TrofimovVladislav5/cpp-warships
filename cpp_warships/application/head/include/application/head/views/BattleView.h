#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/box.hpp>

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/BattleState.h>
#include <application/model/BattleJournal.h>
#include <application/head/views/BoardView.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

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

        /** @brief Whether a screen position falls inside the log, which scrolls on its own. */

        const PresentationContext& context_;
        BoardView ownWatersView_;
        BoardView enemyWatersView_;
        GridGeometry& geometry_;
        ftxui::Box logBox_;
    };
} // namespace cpp_warships::head
