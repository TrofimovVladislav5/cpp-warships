#pragma once

#include <application/model/intents/GameIntent.h>

#include <memory>
#include <string>

namespace cpp_warships::model::scenarios {
    /** @brief A complex thing the player asked for, given as the simple ones it
     * is made of. */
    class IntendedGameScenario {
    public:
        virtual ~IntendedGameScenario() = default;

        /** @brief What this is called, for saying which scenario gave up and where. */
        [[nodiscard]] virtual std::string name() const = 0;

        /** @brief The next step to take, given how @p previous went, or nothing when there is no
         * more to do. */
        [[nodiscard]] virtual intents::GameIntentPointer next(
            const intents::IntentResult& previous
        ) = 0;
    };

    using ScenarioPointer = std::shared_ptr<IntendedGameScenario>;
}  // namespace cpp_warships::model::scenarios
