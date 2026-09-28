#include <application/head/common/input/handlers/SessionHandlers.h>
#include <application/head/common/state/PresentationState.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>
#include <application/model/scenarios/SequenceScenario.h>

#include <utility>
#include <variant>
#include <vector>

namespace cpp_warships::head::common::input::handlers {
    StartMatchHandler::StartMatchHandler(
        HandlerParts parts,
        state::PresentationState& state
    ) noexcept
        : parts_(parts)
        , state_(state) {}

    bool StartMatchHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchStartRequested>(event);
    }

    void StartMatchHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::MatchStartRequested>(event);
        parts_.scenarios.submit(
            model::scenarios::scenarioOf(
                "starting a match",
                parts_.intents.startMatch(asked.boardSize)
            )
        );
        state_.isAtMenu = false;
    }

    ResumeMatchHandler::ResumeMatchHandler(
        state::PresentationState& state,
        MatchInProgressQuery hasMatch
    ) noexcept
        : state_(state)
        , hasMatch_(std::move(hasMatch)) {}

    bool ResumeMatchHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchResumeRequested>(event) && hasMatch_();
    }

    void ResumeMatchHandler::handleEvent(const model::events::GameEvent&) {
        state_.isAtMenu = false;
    }

    LoadMatchHandler::LoadMatchHandler(
        HandlerParts parts,
        state::PresentationState& state,
        SavedMatchQuery hasSavedMatch
    ) noexcept
        : parts_(parts)
        , state_(state)
        , hasSavedMatch_(std::move(hasSavedMatch)) {}

    bool LoadMatchHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchLoadRequested>(event) && hasSavedMatch_();
    }

    void LoadMatchHandler::handleEvent(const model::events::GameEvent&) {
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("loading the match", parts_.intents.loadMatch())
        );
        state_.isAtMenu = false;
    }

    SaveAndQuitHandler::SaveAndQuitHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool SaveAndQuitHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchSaveAndQuitRequested>(event);
    }

    void SaveAndQuitHandler::handleEvent(const model::events::GameEvent&) {
        parts_.scenarios.submit(
            std::make_shared<model::scenarios::SequenceScenario>(
                "saving and leaving",
                std::vector<model::intents::GameIntentPointer>{
                    parts_.intents.saveMatch(),
                    parts_.intents.finishSession()
                }
            )
        );
    }

    ReturnToMenuHandler::ReturnToMenuHandler(state::PresentationState& state) noexcept
        : state_(state) {}

    bool ReturnToMenuHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MenuReturnRequested>(event);
    }

    void ReturnToMenuHandler::handleEvent(const model::events::GameEvent&) {
        state_.isAtMenu = true;
    }

    QuitHandler::QuitHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool QuitHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::SessionQuitRequested>(event);
    }

    void QuitHandler::handleEvent(const model::events::GameEvent&) {
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("finishing the session", parts_.intents.finishSession())
        );
    }
}  // namespace cpp_warships::head::common::input::handlers
