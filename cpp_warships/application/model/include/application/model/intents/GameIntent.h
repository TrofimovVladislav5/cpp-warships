#pragma once

#include <application/model/intents/IntentResult.h>

#include <memory>
#include <string>

namespace cpp_warships::model::intents {
    /** @brief One simple thing the game can be made to do. */
    class GameIntent {
    public:
        virtual ~GameIntent() = default;

        /** @brief What this is called, for saying which step of a scenario went
         * wrong. */
        [[nodiscard]] virtual std::string name() const = 0;

        /** @brief Does it. Throwing is allowed: the processor is what answers
         * for that. */
        [[nodiscard]] virtual IntentResult apply() const = 0;
    };

    using GameIntentPointer = std::shared_ptr<const GameIntent>;
}  // namespace cpp_warships::model::intents
