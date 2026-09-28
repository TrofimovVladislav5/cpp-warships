#include <application/head/common/input/BoundScreenInput.h>

#include <utility>

namespace cpp_warships::head::common::input {
    void BoundScreenInput::bind(InputKeyPointer key) {
        keys_.push_back(std::move(key));
    }

    std::optional<model::events::GameEvent> BoundScreenInput::interpret(const Keystroke& stroke) {
        for (const InputKeyPointer& key : keys_) {
            if (key->matches(stroke)) {
                return key->interpret(stroke);
            }
        }

        return std::nullopt;
    }
}  // namespace cpp_warships::head::common::input
