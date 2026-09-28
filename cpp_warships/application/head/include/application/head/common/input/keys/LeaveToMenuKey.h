#pragma once

#include <application/head/common/input/InputKey.h>

namespace cpp_warships::head::common::input::keys {
    /** @brief Steps back out to the menu, leaving whatever is in play as it stands. */
    class LeaveToMenuKey final : public InputKey {
    public:
        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };
}  // namespace cpp_warships::head::common::input::keys
