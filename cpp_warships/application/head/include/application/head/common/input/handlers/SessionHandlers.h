#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/state/PresentationState.h>
#include <application/model/events/EventHandler.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>

namespace cpp_warships::head {
    /** @brief What every handler is built with: somewhere to get intents from
     * and somewhere to put the scenario made of them. */
    struct HandlerParts {
        const model::IntentFactory& intents;
        model::ScenarioQueue& scenarios;
    };

    /** @brief Starts a fresh match, and leaves the menu behind. */
    class StartMatchHandler final : public model::EventHandler {
       public:
        StartMatchHandler(HandlerParts parts, PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
        PresentationState& state_;
    };

    /** @brief Goes back to the match already in play. Nothing about the game
     * changes, so this asks for no scenario at all: only the interface moves.
     */
    class ResumeMatchHandler final : public model::EventHandler {
       public:
        ResumeMatchHandler(PresentationState& state, MatchInProgressQuery hasMatch) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        PresentationState& state_;
        MatchInProgressQuery hasMatch_;
    };

    /** @brief Picks the saved match back up, and leaves the menu behind. */
    class LoadMatchHandler final : public model::EventHandler {
       public:
        LoadMatchHandler(
            HandlerParts parts,
            PresentationState& state,
            SavedMatchQuery hasSavedMatch
        ) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
        PresentationState& state_;
        SavedMatchQuery hasSavedMatch_;
    };

    /** @brief Puts the match away and then stops, as one thing. */
    class SaveAndQuitHandler final : public model::EventHandler {
       public:
        explicit SaveAndQuitHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
    };

    /** @brief Steps back out to the menu, leaving the match as it stands. */
    class ReturnToMenuHandler final : public model::EventHandler {
       public:
        explicit ReturnToMenuHandler(PresentationState& state) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        PresentationState& state_;
    };

    /** @brief Ends the session, behind whatever was already queued. */
    class QuitHandler final : public model::EventHandler {
       public:
        explicit QuitHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
    };
}  // namespace cpp_warships::head
