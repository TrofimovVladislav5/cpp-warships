#pragma once

#include <application/head/common/ScreenKind.h>
#include <application/head/common/input/ScreenInput.h>
#include <application/model/events/EventScope.h>
#include <application/model/events/GameEvent.h>

#include <map>
#include <memory>
#include <optional>

namespace cpp_warships::head::common::input {
    struct Keystroke;
}

namespace cpp_warships::head::common::input {
    /** @brief The one place raw input becomes something the game can answer. */
    class EventBus {
    public:
        /** @brief Puts @p input in charge of reading @p screen. */
        void readScreenWith(ScreenKind screen, std::unique_ptr<ScreenInput> input);

        /** @brief What @p stroke asks of the game, read as the player being on
         * @p screen. */
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            ScreenKind screen,
            const Keystroke& stroke
        );

    private:
        std::map<ScreenKind, std::unique_ptr<ScreenInput>> inputs_;
    };

    /** @brief When a handler bound to @p screen is listening. */
    [[nodiscard]] model::events::EventScope scopeOf(ScreenKind screen) noexcept;
}  // namespace cpp_warships::head::common::input
