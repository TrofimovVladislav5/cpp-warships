#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/EventBus.h>
#include <application/head/common/input/EventPipeline.h>
#include <application/head/common/input/Keystroke.h>
#include <application/model/events/EventQueue.h>
#include <application/model/events/EventRouter.h>
#include <application/model/scenarios/ScenarioQueue.h>

#include <optional>
#include <utility>
#include <vector>

namespace cpp_warships::head::common::input {
    EventPipeline::EventPipeline(
        PresentationContext& context,
        EventBus& bus,
        model::events::EventQueue& events,
        model::events::EventRouter& router,
        model::scenarios::ScenarioQueue& scenarios
    )
        : context_(context)
        , bus_(bus)
        , events_(events)
        , router_(router)
        , scenarios_(scenarios) {}

    void EventPipeline::offer(const Keystroke& stroke) {
        std::optional<model::events::GameEvent> asked =
            bus_.interpret(context_.currentScreen(), stroke);
        if (asked.has_value()) {
            events_.push(std::move(*asked));
        }
    }

    bool EventPipeline::settle() {
        bool isAnythingClaimed = false;

        for (const model::events::GameEvent& event : events_.drain()) {
            router_.enterScope(scopeOf(context_.currentScreen()));
            isAnythingClaimed = router_.dispatch(event) || isAnythingClaimed;
        }

        scenarios_.start();
        scenarios_.join();

        return isAnythingClaimed;
    }
}  // namespace cpp_warships::head::common::input
