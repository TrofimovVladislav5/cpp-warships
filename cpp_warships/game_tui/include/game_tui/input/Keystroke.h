#pragma once

#include <string>

namespace cpp_warships::game_tui {
    /** @brief A key the player pressed, named for what it means rather than what sent it. */
    enum class Key {
        None,
        Character,
        Enter,
        Escape,
        Tab,
        Backspace,
        Delete,
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight,
        PageUp,
        PageDown,

        /** @brief The function keys the game uses for saving and loading. */
        SaveKey,
        LoadKey,

        Pointer,
    };

    /** @brief Which part of a pointing device was used, when one was used at all. */
    enum class PointerButton {
        None,
        Left,
        Right,
        Middle,
        WheelUp,
        WheelDown,
    };

    /** @brief One thing the player did, in terms no particular terminal library owns.
     *  Whatever is hosting the game translates its own events into these, once. */
    struct Keystroke {
        Key key = Key::None;

        /** @brief What was typed, when a plain character was. */
        std::string character;

        PointerButton button = PointerButton::None;
        bool isPressed = false;
        int pointerX = 0;
        int pointerY = 0;
    };

    /** @brief Whether @p stroke is the given key being typed. */
    [[nodiscard]] bool isCharacter(const Keystroke& stroke, const std::string& character);

    /** @brief Whether @p stroke came from a pointing device at all. */
    [[nodiscard]] bool isPointer(const Keystroke& stroke);

    /** @brief Whether @p stroke is the wheel being rolled either way. */
    [[nodiscard]] bool isWheelRolled(const Keystroke& stroke);
} // namespace cpp_warships::game_tui
