#pragma once

#include <application/head/common/input/EventBus.h>
#include <application/model/events/EventRouter.h>
#include <build_queries.h>

#include <memory>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::model::intents {
    class IntentFactory;
}

namespace cpp_warships::model::scenarios {
    class ScenarioQueue;
}

namespace cpp_warships::application {
    /** @brief The bus that reads each screen, all of it working from @p context. */
    [[nodiscard]] std::unique_ptr<head::common::input::EventBus> buildEventBus(
        head::common::PresentationContext& context
    );

    /** @brief The router, with every handler subscribed to the scope it listens
     * in. This is the one place that says what the game does about each ask. */
    [[nodiscard]] std::unique_ptr<model::events::EventRouter> buildEventRouter(
        const model::intents::IntentFactory& intents,
        model::scenarios::ScenarioQueue& scenarios,
        head::common::PresentationContext& context,
        const SessionQueries& queries
    );
}  // namespace cpp_warships::application
