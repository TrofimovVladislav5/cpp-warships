#include <application/model/intents/IntentFactory.h>

#include <memory>

#include <application/model/intents/MatchIntents.h>
#include <application/model/intents/SessionIntents.h>

namespace cpp_warships::model {
    IntentFactory::IntentFactory(ApplicationContext& application) noexcept
        : application_(application) {}

    GameIntentPointer IntentFactory::startMatch(const int boardSize) const {
        return std::make_shared<StartMatchIntent>(application_.game().play(), boardSize);
    }

    GameIntentPointer IntentFactory::placeShip(
            const core::Coordinate origin,
            const core::Direction direction,
            const int length
    ) const {
        return std::make_shared<PlaceShipIntent>(
                application_.game().play(),
                origin,
                direction,
                length
        );
    }

    GameIntentPointer IntentFactory::removeShip(const core::Coordinate coordinate) const {
        return std::make_shared<RemoveShipIntent>(application_.game().play(), coordinate);
    }

    GameIntentPointer IntentFactory::shuffleFleet() const {
        return std::make_shared<ShuffleFleetIntent>(application_.game().play());
    }

    GameIntentPointer IntentFactory::beginBattle() const {
        return std::make_shared<BeginBattleIntent>(application_.game().play());
    }

    GameIntentPointer IntentFactory::fireAt(const core::Coordinate coordinate) const {
        return std::make_shared<FireAtIntent>(application_.game().play(), coordinate);
    }

    GameIntentPointer IntentFactory::useSkill(
            const std::optional<core::Coordinate> target
    ) const {
        return std::make_shared<UseSkillIntent>(application_.game().play(), target);
    }

    GameIntentPointer IntentFactory::saveMatch() const {
        return std::make_shared<SaveMatchIntent>(application_.game().saves());
    }

    GameIntentPointer IntentFactory::loadMatch() const {
        return std::make_shared<LoadMatchIntent>(application_.game().saves());
    }

    GameIntentPointer IntentFactory::finishSession() const {
        return std::make_shared<FinishSessionIntent>(application_);
    }
} // namespace cpp_warships::model
