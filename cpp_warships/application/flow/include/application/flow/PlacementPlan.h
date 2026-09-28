#pragma once

#include <application/flow/RandomEngine.h>

#include <map>

namespace cpp_warships::core {
    class Board;
    class FleetComposition;
}  // namespace cpp_warships::core

namespace cpp_warships::flow {
    /** @brief What still has to be placed to satisfy a fleet composition.
     * Derived from the board on demand, so the two cannot drift apart. */
    class PlacementPlan {
    public:
        PlacementPlan(const core::FleetComposition& composition, const core::Board& board);

        [[nodiscard]] int remainingOf(int shipLength) const;
        [[nodiscard]] const std::map<int, int>& remaining() const noexcept;
        [[nodiscard]] int remainingShipCount() const;
        [[nodiscard]] bool isComplete() const;

    private:
        std::map<int, int> remaining_;
    };

    /** @brief Clears the board and lays out a whole fleet at random, every segment with @p
     * segmentHealth. */
    bool placeFleetRandomly(
        core::Board& board,
        const core::FleetComposition& composition,
        RandomEngine& randomEngine,
        int segmentHealth
    );
}  // namespace cpp_warships::flow
