#pragma once

#include <memory>

#include <application/model/events/GameEvent.h>

namespace cpp_warships::model {
    /** @brief One thing the game can do in answer to an event.
     *  The handler itself decides whether an event is any of its business, so adding a
     *  way to respond means adding a handler rather than editing a branch somewhere. */
    class EventHandler {
    public:
        virtual ~EventHandler() = default;

        /** @brief Whether @p event is something this handler acts on. */
        [[nodiscard]] virtual bool isHandled(const GameEvent& event) const = 0;

        /** @brief Acts on @p event, having already claimed it through isHandled. */
        virtual void handleEvent(const GameEvent& event) = 0;
    };

    using EventHandlerPointer = std::shared_ptr<EventHandler>;
} // namespace cpp_warships::model
