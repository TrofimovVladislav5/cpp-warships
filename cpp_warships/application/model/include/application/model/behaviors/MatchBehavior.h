#pragma once

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>

#include <optional>

namespace cpp_warships::model {
    class MatchInPlay;
}

namespace cpp_warships::model::behaviors {
    /** @brief Everything that can be done to a match: starting one, laying out a fleet and
     * fighting it out. */
    class MatchBehavior {
    public:
        /** @brief Acts on @p inPlay, which must outlive this behaviour. */
        explicit MatchBehavior(MatchInPlay& inPlay) noexcept;

        void startNewMatch(int boardSize);

        void placeShip(core::Coordinate origin, core::Direction direction, int length);
        void removeShipAt(core::Coordinate coordinate);
        void shuffleFleet();

        /** @brief Opens fire, once the fleet is laid out. @return whether the
         * battle began. */
        bool beginBattle();

        void fireAt(core::Coordinate coordinate);
        void useSkill(std::optional<core::Coordinate> target);

    private:
        /** @brief Lets the match hand play on, then keeps what happened. */
        void settleTurn();

        MatchInPlay& inPlay_;
    };
}  // namespace cpp_warships::model::behaviors
