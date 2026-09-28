#include <application/model/events/EventQueue.h>

#include <utility>

namespace cpp_warships::model {
    void EventQueue::push(GameEvent event) {
        events_.push_back(std::move(event));
    }

    std::vector<GameEvent> EventQueue::drain() {
        std::vector<GameEvent> taken{
            std::make_move_iterator(events_.begin()),
            std::make_move_iterator(events_.end())
        };
        events_.clear();

        return taken;
    }

    bool EventQueue::isEmpty() const noexcept {
        return events_.empty();
    }
}  // namespace cpp_warships::model
