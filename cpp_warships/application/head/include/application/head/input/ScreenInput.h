#pragma once

#include <optional>

#include <application/head/input/Keystroke.h>
#include <application/model/events/GameEvent.h>

namespace cpp_warships::head {
    /** @brief Reads what the player did on one screen and says what they meant by it.
     *  Anything that is the interface's own business, such as walking a cursor or
     *  scrolling a log, is dealt with here and reported as nothing at all. */
    class ScreenInput {
    public:
        virtual ~ScreenInput() = default;

        /** @brief What @p stroke asks of the game, or nothing when it asks the game
         *  for nothing. */
        [[nodiscard]] virtual std::optional<model::GameEvent> interpret(
                const Keystroke& stroke
        ) = 0;
    };
} // namespace cpp_warships::head
