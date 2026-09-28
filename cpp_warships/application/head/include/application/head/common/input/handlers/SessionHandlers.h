#pragma once

#include <application/head/common/Queries.h>
#include <application/model/events/EventHandler.h>

namespace cpp_warships::head::common::state {
    struct PresentationState;
}

namespace cpp_warships::model::intents {
    class IntentFactory;
}

namespace cpp_warships::model::scenarios {
    class ScenarioQueue;
}

namespace cpp_warships::head::common::input::handlers {
    /** @brief What every handler is built with: somewhere to get intents from
     * and somewhere to put the scenario made of them. */
    struct HandlerParts {
        const model::intents::IntentFactory& intents;
        model::scenarios::ScenarioQueue& scenarios;
    };

    /** @brief Starts a fresh match, and leaves the menu behind. */
    class StartMatchHandler final : public model::events::EventHandler {
    public:
        StartMatchHandler(HandlerParts parts, state::PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        state::PresentationState& state_;
    };

    /** @brief Goes back to the match already in play. */
    class ResumeMatchHandler final : public model::events::EventHandler {
    public:
        ResumeMatchHandler(state::PresentationState& state, MatchInProgressQuery hasMatch) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        state::PresentationState& state_;
        MatchInProgressQuery hasMatch_;
    };

    /** @brief Opens the list of saved games, if there is anything in it. */
    class OpenSaveBrowserHandler final : public model::events::EventHandler {
    public:
        OpenSaveBrowserHandler(
            state::PresentationState& state,
            MatchInProgressQuery hasMatch,
            SavedMatchQuery hasSavedMatch
        ) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        state::PresentationState& state_;
        MatchInProgressQuery hasMatch_;
        SavedMatchQuery hasSavedMatch_;
    };

    /** @brief Picks a chosen save back up, and leaves the browser behind. */
    class LoadMatchHandler final : public model::events::EventHandler {
    public:
        LoadMatchHandler(HandlerParts parts, state::PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        state::PresentationState& state_;
    };

    /** @brief Throws a chosen save away, leaving the player in the browser. */
    class DeleteSaveHandler final : public model::events::EventHandler {
    public:
        DeleteSaveHandler(HandlerParts parts, state::PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        state::PresentationState& state_;
    };

    /** @brief Opens the prompt asking what to call the match, filled in with what it is
     * already called when it came from a save. */
    class OpenSaveNamingHandler final : public model::events::EventHandler {
    public:
        OpenSaveNamingHandler(
            state::PresentationState& state,
            MatchInProgressQuery hasMatch,
            SaveNameQuery nameInPlay
        ) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        state::PresentationState& state_;
        MatchInProgressQuery hasMatch_;
        SaveNameQuery nameInPlay_;
    };

    /** @brief Puts the match away and then stops, as one thing. */
    class SaveAndQuitHandler final : public model::events::EventHandler {
    public:
        SaveAndQuitHandler(HandlerParts parts, state::PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        state::PresentationState& state_;
    };

    /** @brief Steps back out to the menu, leaving the match as it stands. */
    class ReturnToMenuHandler final : public model::events::EventHandler {
    public:
        explicit ReturnToMenuHandler(state::PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        state::PresentationState& state_;
    };

    /** @brief Ends the session, behind whatever was already queued. */
    class QuitHandler final : public model::events::EventHandler {
    public:
        explicit QuitHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };
}  // namespace cpp_warships::head::common::input::handlers
