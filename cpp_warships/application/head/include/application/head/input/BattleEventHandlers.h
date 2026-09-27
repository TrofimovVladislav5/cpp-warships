#pragma once

#include <application/head/Queries.h>
#include <application/head/input/EventHandler.h>
#include <application/head/intents/Intent.h>
#include <application/head/screens/BattleState.h>
#include <application/head/session/BattleJournal.h>

namespace cpp_warships::head {
    /** @brief Fires at the cell under the sight, and holds off while the enemy is shooting. */
    class FireEventHandler final : public EventHandler {
    public:
        FireEventHandler(IntentSink intentSink, BattleState& state, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        BattleState& state_;
        MatchQuery match_;
    };

    /** @brief Spends the next banked skill, aiming it where the player is aiming. */
    class UseSkillEventHandler final : public EventHandler {
    public:
        UseSkillEventHandler(IntentSink intentSink, BattleState& state, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        BattleState& state_;
        MatchQuery match_;
    };

    /** @brief Takes aim and fires with the mouse, reading cells off the enemy grid. */
    class BattleMouseEventHandler final : public EventHandler {
    public:
        BattleMouseEventHandler(IntentSink intentSink, BattleState& state, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        BattleState& state_;
        MatchQuery match_;
    };

    /** @brief Walks the log back through older lines, by wheel over it or by page keys. */
    class ScrollLogEventHandler final : public EventHandler {
    public:
        ScrollLogEventHandler(BattleState& state, const BattleJournal& journal);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        BattleState& state_;
        const BattleJournal& journal_;
    };

    /** @brief Goes back to the menu, leaving the match standing where it is. */
    class LeaveBattleEventHandler final : public EventHandler {
    public:
        explicit LeaveBattleEventHandler(IntentSink intentSink);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
    };
} // namespace cpp_warships::head
