#pragma once

#include <application/model/events/GameEvent.h>

#include <deque>
#include <vector>

namespace cpp_warships::model {
    /** @brief Game events in the order they arrived, waiting to be acted on. */
    class EventQueue {
       public:
        void push(GameEvent event);

        /** @brief Takes everything waiting, leaving the queue empty. */
        [[nodiscard]] std::vector<GameEvent> drain();

        [[nodiscard]] bool isEmpty() const noexcept;

       private:
        std::deque<GameEvent> events_;
    };
}  // namespace cpp_warships::model
