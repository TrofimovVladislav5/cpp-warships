#pragma once

#include <vector>

#include <application/model/events/EventHandler.h>
#include <application/model/events/EventScope.h>

namespace cpp_warships::model {
    /** @brief Who is listening for what, and where. Offers an event to the handlers that
     *  are in scope, in the order they subscribed, and stops at the first to claim it. */
    class EventRouter {
    public:
        /** @brief Registers @p handler to hear events while @p scope is the one in play.
         *  A subscription lasts for the session: nothing here takes one back, because
         *  nothing yet needs to. Handing back a token is what that would want. */
        void subscribe(EventScope scope, EventHandlerPointer handler);

        /** @brief Says which part of the game the player is now in. */
        void enterScope(EventScope scope) noexcept;
        [[nodiscard]] EventScope currentScope() const noexcept;

        /** @brief Gives @p event to the first handler in scope that claims it.
         *  @return whether any handler took it. */
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
} // namespace cpp_warships::model
