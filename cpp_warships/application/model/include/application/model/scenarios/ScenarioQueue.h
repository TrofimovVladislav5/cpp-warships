#pragma once

#include <application/model/scenarios/IntendedGameScenario.h>

namespace cpp_warships::model::scenarios {
    /** @brief Scenarios waiting to be played out, and the saying of when that
     * happens. */
    class ScenarioQueue {
    public:
        virtual ~ScenarioQueue() = default;

        /** @brief Adds @p scenario to the back of the queue. */
        virtual void submit(ScenarioPointer scenario) = 0;

        /** @brief Begins playing out everything queued. */
        virtual void start() = 0;

        /** @brief Waits until nothing is left to play out. */
        virtual void join() = 0;

        /** @brief Whether there is nothing queued and nothing in hand. */
        [[nodiscard]] virtual bool isIdle() const = 0;
    };
}  // namespace cpp_warships::model::scenarios
