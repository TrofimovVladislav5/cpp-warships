#include <application/head/common/input/EventPipeline.h>

#include <optional>
#include <utility>
#include <vector>

namespace cpp_warships::head {
    EventPipeline::EventPipeline(
        PresentationContext& context,
        EventBus& bus,
        model::EventQueue& events,
        model::EventRouter& router,
        model::ScenarioQueue& scenarios
    )
        : context_(context), bus_(bus), events_(events), router_(router), scenarios_(scenarios) {
    }

    void EventPipeline::offer(const Keystroke& stroke) {
        std::optional<model::GameEvent> asked = bus_.interpret(context_.currentScreen(), stroke);
        if (asked.has_value()) {
            events_.push(std::move(*asked));
        }
    }

    bool EventPipeline::settle() {
        bool isAnythingClaimed = false;

        for (const model::GameEvent& event : events_.drain()) {
            router_.enterScope(scopeOf(context_.currentScreen()));
            isAnythingClaimed = router_.dispatch(event) || isAnythingClaimed;
        }

        scenarios_.start();
        scenarios_.join();

        return isAnythingClaimed;
    }
}  // namespace cpp_warships::head
