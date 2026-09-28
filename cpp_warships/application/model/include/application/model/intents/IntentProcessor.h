#pragma once

#include <application/model/intents/GameIntent.h>

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::model::scenarios {
    class IntendedGameScenario;
}

namespace cpp_warships::model::intents {
    /** @brief Plays a scenario out, step by step, and answers for everything
     * that goes wrong inside it. */
    class IntentProcessor {
    public:
        /** @brief Runs scenarios against @p application, which must outlive it. */
        explicit IntentProcessor(ApplicationContext& application) noexcept;

        /** @brief Takes @p scenario as far as it goes. */
        void run(scenarios::IntendedGameScenario& scenario);

    private:
        /** @brief One step, with whatever it throws turned into a failed
         * result. */
        [[nodiscard]] IntentResult applyStep(const GameIntent& step);

        ApplicationContext& application_;
    };
}  // namespace cpp_warships::model::intents
