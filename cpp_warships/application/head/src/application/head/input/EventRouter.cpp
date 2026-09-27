#include <application/head/input/EventRouter.h>

#include <algorithm>
#include <utility>

namespace cpp_warships::head {

    void EventRouter::add(EventHandlerPointer handler) {
        if (handler) {
            handlers_.push_back(std::move(handler));
        }
    }

    bool EventRouter::dispatch(const InputEvent& input) {
        const auto claimsEvent = [&input](const EventHandlerPointer& handler) {
            return handler->isHandled(input);
        };

        const auto handlersFound = std::find_if(handlers_.begin(), handlers_.end(), claimsEvent);

        if (handlersFound == handlers_.end()) {
            return false;
        } else {
            (*handlersFound)->handleEvent(input);
            return true;
        }
    }
} // namespace cpp_warships::head
