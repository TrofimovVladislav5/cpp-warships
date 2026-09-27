#pragma once

#include <memory>

#include <application/head/input/InputEvent.h>

namespace cpp_warships::head {
    /** @brief One thing a screen can do in response to input.
     *  The handler itself decides whether an event is any of its business. */
    class EventHandler {
    public:
        virtual ~EventHandler() = default;

        /** @brief Whether @p input is something this handler acts on. */
        [[nodiscard]] virtual bool isHandled(const InputEvent& input) const = 0;

        /** @brief Acts on @p input, having already claimed it through isHandled. */
        virtual void handleEvent(const InputEvent& input) = 0;
    };

    using EventHandlerPointer = std::shared_ptr<EventHandler>;
} // namespace cpp_warships::head
