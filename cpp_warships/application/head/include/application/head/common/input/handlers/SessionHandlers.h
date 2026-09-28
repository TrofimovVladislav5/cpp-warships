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

    /** @brief Picks the saved match back up, and leaves the menu behind. */
    class LoadMatchHandler final : public model::events::EventHandler {
    public:
        LoadMatchHandler(
            HandlerParts parts,
            state::PresentationState& state,
            SavedMatchQuery hasSavedMatch
        ) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        state::PresentationState& state_;
        SavedMatchQuery hasSavedMatch_;
    };

    /** @brief Puts the match away and then stops, as one thing. */
    class SaveAndQuitHandler final : public model::events::EventHandler {
    public:
        explicit SaveAndQuitHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
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
