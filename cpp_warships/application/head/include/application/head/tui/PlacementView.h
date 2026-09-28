#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/common/Queries.h>
#include <application/head/common/Theme.h>
#include <application/head/common/state/PlacementState.h>
#include <application/head/tui/BoardView.h>
#include <application/head/tui/FtxuiView.h>

#include <ftxui/dom/elements.hpp>
#include <optional>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::tui {
    /** @brief Draws the player's board beside the fleet still waiting to be
     * laid out. Reads the match and returns elements; it changes nothing. */
    class PlacementView final : public FtxuiRenderer {
    public:
        PlacementView(
            const common::PresentationContext& context,
            common::input::GridGeometry& geometry
        ) noexcept;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const common::PresentationContext& context_;
        BoardView boardView_;
    };
}  // namespace cpp_warships::head::tui
