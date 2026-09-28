#pragma once

#include <application/model/events/GameEvent.h>

#include <memory>

namespace cpp_warships::model::events {
    /** @brief One thing the game can do in answer to an event. */
    class EventHandler {
    public:
        virtual ~EventHandler() = default;

        /** @brief Whether @p event is something this handler acts on. */
        [[nodiscard]] virtual bool isHandled(const GameEvent& event) const = 0;

        /** @brief Acts on @p event, having already claimed it through
         * isHandled. */
        virtual void handleEvent(const GameEvent& event) = 0;
    };

    using EventHandlerPointer = std::shared_ptr<EventHandler>;
}  // namespace cpp_warships::model::events
