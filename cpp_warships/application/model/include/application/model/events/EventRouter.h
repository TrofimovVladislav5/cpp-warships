#pragma once

#include <application/model/events/EventHandler.h>
#include <application/model/events/EventScope.h>

#include <vector>

namespace cpp_warships::model::events {
    /** @brief Who is listening for what, and where. */
    class EventRouter {
    public:
        /** @brief Registers @p handler to hear events while @p scope is the one
         * in play. */
        void subscribe(EventScope scope, EventHandlerPointer handler);

        /** @brief Says which part of the game the player is now in. */
        void enterScope(EventScope scope) noexcept;
        [[nodiscard]] EventScope currentScope() const noexcept;

        /** @brief Gives @p event to the first handler in scope that claims it.
         * @return whether any handler took it. */
        bool dispatch(const GameEvent& event);

    private:
        struct Subscription {
            EventScope scope;
            EventHandlerPointer handler;
        };

        [[nodiscard]] bool isInScope(EventScope scope) const noexcept;

        std::vector<Subscription> subscriptions_;
        EventScope currentScope_ = EventScope::Menu;
    };
}  // namespace cpp_warships::model::events
