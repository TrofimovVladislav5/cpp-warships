#pragma once

#include <memory>

#include <build_queries.h>

#include <application/head/PresentationContext.h>
#include <application/head/input/EventBus.h>
#include <application/model/events/EventRouter.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/scenarios/ScenarioQueue.h>

namespace cpp_warships::application {
    /** @brief The bus that reads each screen, all of it working from @p context.
     *  This is the one place that says which key means what on which screen. */
    [[nodiscard]] std::unique_ptr<head::EventBus> buildEventBus(head::PresentationContext& context);

    /** @brief The router, with every handler subscribed to the scope it listens in.
     *  This is the one place that says what the game does about each ask. */
    [[nodiscard]] std::unique_ptr<model::EventRouter> buildEventRouter(
            const model::IntentFactory& intents,
            model::ScenarioQueue& scenarios,
            head::PresentationContext& context,
            const SessionQueries& queries
    );
} // namespace cpp_warships::application
