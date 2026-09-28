#pragma once

#include <application/head/common/input/InputKey.h>
#include <application/head/common/input/ScreenInput.h>

#include <vector>

namespace cpp_warships::head::common::input {
    /** @brief A screen read through its bindings: the first one that answers to a stroke
     * is the one that says what it meant. */
    class BoundScreenInput : public ScreenInput {
    public:
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) final;

    protected:
        /** @brief Adds @p key behind everything bound before it. */
        void bind(InputKeyPointer key);

    private:
        std::vector<InputKeyPointer> keys_;
    };
}  // namespace cpp_warships::head::common::input
