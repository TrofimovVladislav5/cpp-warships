#pragma once

#include <application/model/events/GameEvent.h>

#include <optional>

namespace cpp_warships::head::common::input {
    struct Keystroke;
}

namespace cpp_warships::head::common::input {
    /** @brief Reads what the player did on one screen and says what they meant
     * by it. */
    class ScreenInput {
    public:
        virtual ~ScreenInput() = default;

        /** @brief What @p stroke asks of the game, or nothing when it asks the
         * game for nothing. */
        [[nodiscard]] virtual std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) = 0;
    };
}  // namespace cpp_warships::head::common::input
