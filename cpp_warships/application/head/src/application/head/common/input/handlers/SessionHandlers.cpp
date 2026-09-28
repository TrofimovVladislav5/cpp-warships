#include <application/head/common/input/handlers/SessionHandlers.h>
#include <application/head/common/state/PresentationState.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>
#include <application/model/scenarios/SequenceScenario.h>

#include <algorithm>
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

    OpenSaveBrowserHandler::OpenSaveBrowserHandler(
        state::PresentationState& state,
        MatchInProgressQuery hasMatch,
        SavedMatchQuery hasSavedMatch
    ) noexcept
        : state_(state)
        , hasMatch_(std::move(hasMatch))
        , hasSavedMatch_(std::move(hasSavedMatch)) {}

    bool OpenSaveBrowserHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::SaveBrowserRequested>(event) && !hasMatch_() &&
               hasSavedMatch_();
    }

    void OpenSaveBrowserHandler::handleEvent(const model::events::GameEvent&) {
        state_.isBrowsingSaves = true;
        state_.saves.selectedIndex = 0;
    }

    LoadMatchHandler::LoadMatchHandler(HandlerParts parts, state::PresentationState& state) noexcept
        : parts_(parts)
        , state_(state) {}

    bool LoadMatchHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchLoadRequested>(event);
    }

    void LoadMatchHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::MatchLoadRequested>(event);
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("loading the match", parts_.intents.loadMatch(asked.name))
        );
        state_.isBrowsingSaves = false;
        state_.isAtMenu = false;
    }

    DeleteSaveHandler::DeleteSaveHandler(
        HandlerParts parts,
        state::PresentationState& state
    ) noexcept
        : parts_(parts)
        , state_(state) {}

    bool DeleteSaveHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::SaveDeleteRequested>(event);
    }

    void DeleteSaveHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::SaveDeleteRequested>(event);
        parts_.scenarios.submit(
            model::scenarios::scenarioOf("deleting a save", parts_.intents.deleteSave(asked.name))
        );
        state_.saves.selectedIndex = std::max(0, state_.saves.selectedIndex - 1);
    }

    OpenSaveNamingHandler::OpenSaveNamingHandler(
        state::PresentationState& state,
        MatchInProgressQuery hasMatch,
        SaveNameQuery nameInPlay
    ) noexcept
        : state_(state)
        , hasMatch_(std::move(hasMatch))
        , nameInPlay_(std::move(nameInPlay)) {}

    bool OpenSaveNamingHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::SaveNamingRequested>(event) && hasMatch_();
    }

    void OpenSaveNamingHandler::handleEvent(const model::events::GameEvent&) {
        state_.isNamingSave = true;
        state_.naming.typedName = nameInPlay_();
    }

    SaveAndQuitHandler::SaveAndQuitHandler(
        HandlerParts parts,
        state::PresentationState& state
    ) noexcept
        : parts_(parts)
        , state_(state) {}

    bool SaveAndQuitHandler::isHandled(const model::events::GameEvent& event) const {
        return model::events::isKind<model::events::MatchSaveAndQuitRequested>(event);
    }

    void SaveAndQuitHandler::handleEvent(const model::events::GameEvent& event) {
        const auto& asked = std::get<model::events::MatchSaveAndQuitRequested>(event);
        state_.isNamingSave = false;
        parts_.scenarios.submit(
            std::make_shared<model::scenarios::SequenceScenario>(
                "saving and leaving",
                std::vector<model::intents::GameIntentPointer>{
                    parts_.intents.saveMatch(asked.name),
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
        state_.isBrowsingSaves = false;
        state_.isNamingSave = false;
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
