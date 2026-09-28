#include <application/core/MatchSettings.h>
#include <application/model/behaviors/MatchBehavior.h>

namespace cpp_warships::model {
    MatchBehavior::MatchBehavior(MatchInPlay& inPlay) noexcept : inPlay_(inPlay) {
    }

    void MatchBehavior::startNewMatch(const int boardSize) {
        inPlay_.replaceWith(
            flow::Match{core::MatchSettings::forBoardSize(boardSize), inPlay_.randomEngine()}
        );
    }

    void MatchBehavior::placeShip(
        const core::Coordinate origin,
        const core::Direction direction,
        const int length
    ) {
        if (!inPlay_.hasMatch()) {
            return;
        }

        flow::Match& match = inPlay_.editableMatch();
        match.editablePlayerBoard()
            .place(origin, direction, length, match.settings().segmentHealth());
    }

    void MatchBehavior::removeShipAt(const core::Coordinate coordinate) {
        if (!inPlay_.hasMatch()) {
            return;
        }

        inPlay_.editableMatch().editablePlayerBoard().removeShipAt(coordinate);
    }

    void MatchBehavior::shuffleFleet() {
        if (!inPlay_.hasMatch()) {
            return;
        }

        inPlay_.editableMatch().shufflePlayerFleet();
    }

    bool MatchBehavior::beginBattle() {
        if (!inPlay_.hasMatch() || !inPlay_.editableMatch().beginBattle()) {
            return false;
        }

        inPlay_.recordEvents();
        return true;
    }

    void MatchBehavior::fireAt(const core::Coordinate coordinate) {
        if (!inPlay_.hasMatch()) {
            return;
        }

        inPlay_.editableMatch().fireAt(coordinate);
        settleTurn();
    }

    void MatchBehavior::useSkill(const std::optional<core::Coordinate> target) {
        if (!inPlay_.hasMatch()) {
            return;
        }

        inPlay_.editableMatch().applyNextSkill(target);
        settleTurn();
    }

    void MatchBehavior::settleTurn() {
        inPlay_.editableMatch().concludeTurn();
        inPlay_.recordEvents();
    }
}  // namespace cpp_warships::model
