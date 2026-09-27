#pragma once

#include <memory>
#include <string>

#include <application/model/intents/GameIntent.h>

namespace cpp_warships::model {
    /** @brief A complex thing the player asked for, given as the simple ones it is made of.
     *  It hands them over one at a time and is told how each went, so what comes next may
     *  depend on what just happened. Ordering lives here, where it can be read, rather
     *  than in a table of numbers somewhere else. */
    class IntendedGameScenario {
    public:
        virtual ~IntendedGameScenario() = default;

        /** @brief What this is called, for saying which scenario gave up and where. */
        [[nodiscard]] virtual std::string name() const = 0;

        /** @brief The next step to take, given how @p previous went, or nothing when
         *  there is no more to do. The first call is given a succeeded result. */
        [[nodiscard]] virtual GameIntentPointer next(const IntentResult& previous) = 0;
    };

    using ScenarioPointer = std::shared_ptr<IntendedGameScenario>;
} // namespace cpp_warships::model
