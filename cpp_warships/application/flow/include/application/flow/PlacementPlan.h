#pragma once

#include <map>

#include <application/core/Board.h>
#include <application/core/FleetComposition.h>
#include <application/flow/RandomEngine.h>

namespace cpp_warships::flow {
    /** @brief What still has to be placed to satisfy a fleet composition.
     *  Derived from the board on demand, so the two cannot drift apart. */
    class PlacementPlan {
    public:
        PlacementPlan(core::FleetComposition composition, const core::Board& board);

        [[nodiscard]] int remainingOf(int shipLength) const;
        [[nodiscard]] const std::map<int, int>& remaining() const noexcept;
        [[nodiscard]] int remainingShipCount() const;
        [[nodiscard]] bool isComplete() const;

    private:
        std::map<int, int> remaining_;
    };

    /** @brief Clears the board and lays out a whole fleet at random.
     *  @return false when no legal layout was found, leaving the board empty. */
    bool placeFleetRandomly(
            core::Board& board,
            const core::FleetComposition& composition,
            RandomEngine& randomEngine
    );
} // namespace cpp_warships::flow
