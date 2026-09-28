#pragma once

#include <application/model/events/GameEvent.h>

#include <memory>
#include <optional>

namespace cpp_warships::head::common::input {
    struct Keystroke;
}

namespace cpp_warships::head::common::input {
    /** @brief One binding: the stroke it answers to, and what it makes of that. */
    class InputKey {
    public:
        virtual ~InputKey() = default;

        /** @brief Whether @p stroke is the one this binding answers to. */
        [[nodiscard]] virtual bool matches(const Keystroke& stroke) const = 0;

        /** @brief What @p stroke asks of the game, or nothing when it asks it for nothing. */
        [[nodiscard]] virtual std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) = 0;
    };

    using InputKeyPointer = std::unique_ptr<InputKey>;
}  // namespace cpp_warships::head::common::input
