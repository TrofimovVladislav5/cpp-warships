#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/common/PresentationContext.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/PlacementState.h>
#include <application/head/ftxui/BoardView.h>
#include <application/head/ftxui/FtxuiView.h>

#include <ftxui/dom/elements.hpp>
#include <optional>

namespace cpp_warships::head {
    /** @brief Draws the player's board beside the fleet still waiting to be
     * laid out. Reads the match and returns elements; it changes nothing. */
    class PlacementView final : public FtxuiRenderer {
       public:
        PlacementView(const PresentationContext& context, GridGeometry& geometry) noexcept;

       protected:
        [[nodiscard]] ftxui::Element renderElement() override;

       private:
        const PresentationContext& context_;
        BoardView boardView_;
    };
}  // namespace cpp_warships::head
