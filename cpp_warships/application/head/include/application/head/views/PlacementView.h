#pragma once

#include <optional>

#include <ftxui/dom/elements.hpp>

#include <application/core/Coordinate.h>
#include <application/flow/Match.h>
#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/PlacementState.h>
#include <application/head/views/BoardView.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::head {
    /** @brief Draws the player's board beside the fleet still waiting to be laid out.
     *  Reads the match and returns elements; it changes nothing. */
    class PlacementView final : public FtxuiRenderer {
    public:
        PlacementView(const PresentationContext& context, GridGeometry& geometry) noexcept;


    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const PresentationContext& context_;
        BoardView boardView_;
    };
} // namespace cpp_warships::head
