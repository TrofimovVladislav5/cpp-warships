#include <game_tui/input/Keystroke.h>

namespace cpp_warships::game_tui {
    bool isCharacter(const Keystroke& stroke, const std::string& character) {
        return stroke.key == Key::Character && stroke.character == character;
    }

    bool isPointer(const Keystroke& stroke) {
        return stroke.key == Key::Pointer;
    }

    bool isWheelRolled(const Keystroke& stroke) {
        return isPointer(stroke) && (stroke.button == PointerButton::WheelUp ||
                                     stroke.button == PointerButton::WheelDown);
    }
} // namespace cpp_warships::game_tui
