#include <application/head/input/handlers/SessionHandlers.h>

#include <utility>
#include <variant>

#include <application/model/scenarios/SequenceScenario.h>

namespace cpp_warships::head {
    StartMatchHandler::StartMatchHandler(HandlerParts parts, PresentationState& state) noexcept
        : parts_(parts)
        , state_(state) {}

    bool StartMatchHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::MatchStartRequested>(event);
    }

    void StartMatchHandler::handleEvent(const model::GameEvent& event) {
        const auto& asked = std::get<model::MatchStartRequested>(event);
        parts_.scenarios.submit(
                model::scenarioOf("starting a match", parts_.intents.startMatch(asked.boardSize))
        );
        state_.isAtMenu = false;
    }

    ResumeMatchHandler::ResumeMatchHandler(
            PresentationState& state,
            MatchInProgressQuery hasMatch
    ) noexcept
        : state_(state)
        , hasMatch_(std::move(hasMatch)) {}

    bool ResumeMatchHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::MatchResumeRequested>(event) && hasMatch_();
    }

    void ResumeMatchHandler::handleEvent(const model::GameEvent&) {
        state_.isAtMenu = false;
    }

    LoadMatchHandler::LoadMatchHandler(
            HandlerParts parts,
            PresentationState& state,
            SavedMatchQuery hasSavedMatch
    ) noexcept
        : parts_(parts)
        , state_(state)
        , hasSavedMatch_(std::move(hasSavedMatch)) {}

    bool LoadMatchHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::MatchLoadRequested>(event) && hasSavedMatch_();
    }

    void LoadMatchHandler::handleEvent(const model::GameEvent&) {
        parts_.scenarios.submit(
                model::scenarioOf("loading the match", parts_.intents.loadMatch())
        );
        state_.isAtMenu = false;
    }

    SaveMatchHandler::SaveMatchHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool SaveMatchHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::MatchSaveRequested>(event);
    }

    void SaveMatchHandler::handleEvent(const model::GameEvent&) {
        parts_.scenarios.submit(
                model::scenarioOf("saving the match", parts_.intents.saveMatch())
        );
    }

    ReturnToMenuHandler::ReturnToMenuHandler(PresentationState& state) noexcept
        : state_(state) {}

    bool ReturnToMenuHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::MenuReturnRequested>(event);
    }

    void ReturnToMenuHandler::handleEvent(const model::GameEvent&) {
        state_.isAtMenu = true;
    }

    QuitHandler::QuitHandler(HandlerParts parts) noexcept
        : parts_(parts) {}

    bool QuitHandler::isHandled(const model::GameEvent& event) const {
        return model::isKind<model::SessionQuitRequested>(event);
    }

    void QuitHandler::handleEvent(const model::GameEvent&) {
        parts_.scenarios.submit(
                model::scenarioOf("finishing the session", parts_.intents.finishSession())
        );
    }
} // namespace cpp_warships::head
