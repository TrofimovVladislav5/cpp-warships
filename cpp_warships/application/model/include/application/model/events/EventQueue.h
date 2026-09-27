#pragma once

#include <deque>
#include <vector>

#include <application/model/events/GameEvent.h>

namespace cpp_warships::model {
    /** @brief Game events in the order they arrived, waiting to be acted on.
     *  Keeping them rather than acting at once is what lets a tick settle one thing fully
     *  before the next, which is what quitting behind a save depends on. */
    class EventQueue {
    public:
        void push(GameEvent event);

        /** @brief Takes everything waiting, leaving the queue empty.
         *  Handing them all over at once means an event raised while these are acted on
         *  waits for the next tick rather than lengthening this one. */
        [[nodiscard]] std::vector<GameEvent> drain();

        [[nodiscard]] bool isEmpty() const noexcept;

    private:
        std::deque<GameEvent> events_;
    };
} // namespace cpp_warships::model
