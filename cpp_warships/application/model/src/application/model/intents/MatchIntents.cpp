#include <application/model/behaviors/MatchBehavior.h>
#include <application/model/intents/MatchIntents.h>

namespace cpp_warships::model::intents {
    StartMatchIntent::StartMatchIntent(behaviors::MatchBehavior& play, const int boardSize) noexcept
        : play_(play)
        , boardSize_(boardSize) {}

    std::string StartMatchIntent::name() const {
        return "starting a match";
    }

    IntentResult StartMatchIntent::apply() const {
        play_.startNewMatch(boardSize_);
        return IntentResult::succeeded();
    }

    PlaceShipIntent::PlaceShipIntent(
        behaviors::MatchBehavior& play,
        const core::Coordinate origin,
        const core::Direction direction,
        const int length
    ) noexcept
        : play_(play)
        , origin_(origin)
        , direction_(direction)
        , length_(length) {}

    std::string PlaceShipIntent::name() const {
        return "laying a ship";
    }

    IntentResult PlaceShipIntent::apply() const {
        play_.placeShip(origin_, direction_, length_);
        return IntentResult::succeeded();
    }

    RemoveShipIntent::RemoveShipIntent(
        behaviors::MatchBehavior& play,
        const core::Coordinate coordinate
    ) noexcept
        : play_(play)
        , coordinate_(coordinate) {}

    std::string RemoveShipIntent::name() const {
        return "taking a ship back";
    }

    IntentResult RemoveShipIntent::apply() const {
        play_.removeShipAt(coordinate_);
        return IntentResult::succeeded();
    }

    ShuffleFleetIntent::ShuffleFleetIntent(behaviors::MatchBehavior& play) noexcept
        : play_(play) {}

    std::string ShuffleFleetIntent::name() const {
        return "shuffling the fleet";
    }

    IntentResult ShuffleFleetIntent::apply() const {
        play_.shuffleFleet();
        return IntentResult::succeeded();
    }

    BeginBattleIntent::BeginBattleIntent(behaviors::MatchBehavior& play) noexcept
        : play_(play) {}

    std::string BeginBattleIntent::name() const {
        return "opening fire";
    }

    IntentResult BeginBattleIntent::apply() const {
        if (!play_.beginBattle()) {
            return IntentResult::failed("the fleet is not laid out yet");
        }

        return IntentResult::succeeded();
    }

    FireAtIntent::FireAtIntent(
        behaviors::MatchBehavior& play,
        const core::Coordinate coordinate
    ) noexcept
        : play_(play)
        , coordinate_(coordinate) {}

    std::string FireAtIntent::name() const {
        return "firing";
    }

    IntentResult FireAtIntent::apply() const {
        play_.fireAt(coordinate_);
        return IntentResult::succeeded();
    }

    UseSkillIntent::UseSkillIntent(
        behaviors::MatchBehavior& play,
        std::optional<core::Coordinate> target
    ) noexcept
        : play_(play)
        , target_(target) {}

    std::string UseSkillIntent::name() const {
        return "using a skill";
    }

    IntentResult UseSkillIntent::apply() const {
        play_.useSkill(target_);
        return IntentResult::succeeded();
    }
}  // namespace cpp_warships::model::intents
