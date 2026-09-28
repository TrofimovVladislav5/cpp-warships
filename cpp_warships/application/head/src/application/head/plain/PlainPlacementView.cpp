#include <application/core/Ship.h>
#include <application/head/common/PresentationContext.h>
#include <application/head/common/render/CoordinateLabel.h>
#include <application/head/common/render/Notices.h>
#include <application/head/plain/PlainBoardGrid.h>
#include <application/head/plain/PlainFrame.h>
#include <application/head/plain/PlainPlacementView.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head::plain {
    namespace {
        /** @brief The cells the ship in hand would take up, so they can be
         * marked on the grid. */
        [[nodiscard]] std::unordered_set<core::Coordinate> shipInHandCells(
            const common::state::PlacementState& state,
            const int lengthInHand
        ) {
            if (lengthInHand <= 0) {
                return {};
            }

            const core::Ship shipInHand{state.cursor, state.direction, lengthInHand};
            const std::vector<core::Coordinate> covered = shipInHand.coordinates();
            return {covered.begin(), covered.end()};
        }

        [[nodiscard]] std::vector<std::string> rosterLines(
            const flow::PlacementPlan& plan,
            const int lengthInHand
        ) {
            std::vector<std::string> lines{"FLEET WAITING"};

            for (const auto& [length, remaining] : plan.remaining()) {
                const std::string marker = length == lengthInHand ? " <- in hand" : "";
                lines.push_back(
                    "  length " + std::to_string(length) + " : " + std::to_string(remaining) +
                    " left" + marker
                );
            }

            return lines;
        }
    }  // namespace

    /** @brief The theme is offered and not taken: printed text is not dressed
     * in colour. */
    PlainPlacementView::PlainPlacementView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    common::render::Frame PlainPlacementView::render(int, int) {
        const flow::Match& match = context_.game().match();
        const common::state::PlacementState& state = context_.state().placement;
        const flow::PlacementPlan plan = match.playerPlacementPlan();
        const int lengthInHand = common::state::shipLengthInHand(plan, state);

        const bool isLegal =
            lengthInHand > 0 &&
            match.playerBoard().canPlace(state.cursor, state.direction, lengthInHand) ==
                core::PlacementError::None;

        const PlainBoardOverlay overlay{
            .cursor = state.cursor,
            .marked = shipInHandCells(state, lengthInHand),
            .markGlyph = isLegal ? '+' : '!'
        };

        std::vector<std::string> lines{"PLACE YOUR FLEET", ""};

        const std::vector<std::string> board =
            plainBoardLines(match.playerBoard(), core::Visibility::Owner, overlay);
        lines.insert(lines.end(), board.begin(), board.end());

        lines.emplace_back("");
        lines.push_back(plainBoardLegend());
        lines.emplace_back("");

        const std::vector<std::string> roster = rosterLines(plan, lengthInHand);
        lines.insert(lines.end(), roster.begin(), roster.end());

        const std::string lie = state.direction == core::Direction::Horizontal ? "across" : "down";
        lines.emplace_back("");
        lines.push_back(
            "  aiming at " + common::render::coordinateLabel(state.cursor) + ", lying " + lie +
            (isLegal ? "" : "   (will not fit here)")
        );
        lines.emplace_back("");

        lines.push_back(plainKeyLine("arrows", "aim"));
        lines.push_back(plainKeyLine("enter", "lay the ship"));
        lines.push_back(plainKeyLine("back", "take it back"));
        lines.push_back(plainKeyLine("r", "turn it"));
        lines.push_back(plainKeyLine("tab", "another ship"));
        lines.push_back(plainKeyLine("f", "shuffle the fleet"));

        if (plan.isComplete()) {
            lines.push_back(plainKeyLine("b", "begin the battle"));
        }

        lines.push_back(plainKeyLine("esc", "back to the menu"));
        lines.emplace_back("");

        for (const std::string& notice : common::render::noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return common::render::frameOfLines(lines);
    }
}  // namespace cpp_warships::head::plain
