#pragma once

#include <application/model/ApplicationContext.h>
#include <application/model/scenarios/IntendedGameScenario.h>

namespace cpp_warships::model {
    /** @brief Plays a scenario out, step by step, and answers for everything that goes
     *  wrong inside it. Nothing thrown below this gets past it: an error becomes a failed
     *  result and a notice on the context, and the scenario stops where it stood.
     *  That is what lets the interface hold no catch of its own. */
    class IntentProcessor {
    public:
        /** @brief Runs scenarios against @p application, which must outlive it. */
        explicit IntentProcessor(ApplicationContext& application) noexcept;

        /** @brief Takes @p scenario as far as it goes. */
        void run(IntendedGameScenario& scenario);

    private:
        /** @brief One step, with whatever it throws turned into a failed result. */
        [[nodiscard]] IntentResult applyStep(const GameIntent& step);

        ApplicationContext& application_;
    };
} // namespace cpp_warships::model
