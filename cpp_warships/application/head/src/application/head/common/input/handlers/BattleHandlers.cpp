#include <application/head/common/input/handlers/BattleHandlers.h>
#include <application/model/scenarios/SequenceScenario.h>

#include <optional>
#include <utility>
#include <variant>

namespace cpp_warships::head {
    namespace {
        [[nodiscard]] bool isPlayerFree(const flow::Match& match) {
            return match.phase() == flow::MatchPhase::Battle && match.isPlayerTurn();
        }
    }  // namespace

    FireHandler::FireHandler(HandlerParts parts, MatchQuery match) noexcept
        : parts_(parts), match_(std::move(match)) {
    }

    bool FireHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::ShotRequested>(event) && isPlayerFree(match_());
    }

    void FireHandler::handleEvent(const model::GameEvent& event) {
        const auto& asked = std::get<model::ShotRequested>(event);
        parts_.scenarios.submit(model::scenarioOf("firing", parts_.intents.fireAt(asked.target)));
    }

    UseSkillHandler::UseSkillHandler(HandlerParts parts, MatchQuery match) noexcept
        : parts_(parts), match_(std::move(match)) {
    }

    bool UseSkillHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::SkillUseRequested>(event) && isPlayerFree(match_()) &&
               !match_().skills().isEmpty();
    }

    void UseSkillHandler::handleEvent(const model::GameEvent& event) {
        const auto& asked = std::get<model::SkillUseRequested>(event);
        const std::optional<core::Coordinate> target =
            match_().nextSkillNeedsTarget() ? asked.aim : std::nullopt;

        parts_.scenarios.submit(
            model::scenarioOf("using a skill", parts_.intents.useSkill(target))
        );
    }
}  // namespace cpp_warships::head
