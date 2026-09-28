#include <application/head/common/input/Keystroke.h>
#include <application/head/common/input/keys/LeaveToMenuKey.h>

namespace cpp_warships::head::common::input::keys {
    bool LeaveToMenuKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::Escape;
    }

    std::optional<model::events::GameEvent> LeaveToMenuKey::interpret(const Keystroke&) {
        return model::events::MenuReturnRequested{};
    }
}  // namespace cpp_warships::head::common::input::keys
