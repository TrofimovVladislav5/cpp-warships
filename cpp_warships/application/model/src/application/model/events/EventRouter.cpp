#include <application/model/events/EventRouter.h>

#include <utility>

namespace cpp_warships::model {
    void EventRouter::subscribe(const EventScope scope, EventHandlerPointer handler) {
        subscriptions_.push_back({.scope = scope, .handler = std::move(handler)});
    }

    void EventRouter::enterScope(const EventScope scope) noexcept {
        currentScope_ = scope;
    }

    EventScope EventRouter::currentScope() const noexcept {
        return currentScope_;
    }

    bool EventRouter::dispatch(const GameEvent& event) {
        for (const Subscription& subscription : subscriptions_) {
            if (!isInScope(subscription.scope) || !subscription.handler->isHandled(event)) {
                continue;
            }

            subscription.handler->handleEvent(event);
            return true;
        }

        return false;
    }

    bool EventRouter::isInScope(const EventScope scope) const noexcept {
        return scope == EventScope::Always || scope == currentScope_;
    }
} // namespace cpp_warships::model
