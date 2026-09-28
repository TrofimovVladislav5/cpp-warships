#pragma once

#include <application/model/scenarios/IntendedGameScenario.h>

#include <cstddef>
#include <vector>

namespace cpp_warships::model {
    /** @brief The plainest scenario there is: these steps, in this order,
     * stopping at the first one that does not come off. Most of what a player
     * asks for is this shape. */
    class SequenceScenario final : public IntendedGameScenario {
       public:
        SequenceScenario(std::string name, std::vector<GameIntentPointer> steps);

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] GameIntentPointer next(const IntentResult& previous) override;

       private:
        std::string name_;
        std::vector<GameIntentPointer> steps_;
        std::size_t taken_ = 0;
    };

    /** @brief A scenario of one step, which is what most asks turn out to be.
     */
    [[nodiscard]] ScenarioPointer scenarioOf(std::string name, GameIntentPointer step);
}  // namespace cpp_warships::model
