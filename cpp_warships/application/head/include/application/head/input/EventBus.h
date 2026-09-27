#pragma once

#include <map>
#include <memory>
#include <optional>

#include <application/head/input/Keystroke.h>
#include <application/head/input/ScreenInput.h>
#include <application/head/screens/ScreenKind.h>
#include <application/model/events/EventScope.h>
#include <application/model/events/GameEvent.h>

namespace cpp_warships::head {
    /** @brief The one place raw input becomes something the game can answer. Every question
     *  of which key, which pixel and which cursor is settled here; what leaves is an ask in
     *  the game's own words, or nothing, when the stroke was the interface's own business. */
    class EventBus {
    public:
        /** @brief Puts @p input in charge of reading @p screen. */
        void readScreenWith(ScreenKind screen, std::unique_ptr<ScreenInput> input);

        /** @brief What @p stroke asks of the game, read as the player being on @p screen. */
        [[nodiscard]] std::optional<model::GameEvent> interpret(
                ScreenKind screen,
                const Keystroke& stroke
        );

    private:
        std::map<ScreenKind, std::unique_ptr<ScreenInput>> inputs_;
    };

    /** @brief When a handler bound to @p screen is listening. */
    [[nodiscard]] model::EventScope scopeOf(ScreenKind screen) noexcept;
} // namespace cpp_warships::head
