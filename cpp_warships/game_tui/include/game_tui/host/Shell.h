#pragma once

namespace cpp_warships::game_tui {
    class ScreenNavigator;

    /** @brief Whatever is hosting the interface, seen only as the thing that can be closed.
     *  Keeps the terminal loop out of everything that merely wants to end it. */
    class Shell {
    public:
        virtual ~Shell() = default;

        /** @brief Asks the host to stop showing the interface and return. */
        virtual void requestQuit() = 0;

        /** @brief Shows whichever screen @p navigator has open, until the player quits. */
        virtual void run(ScreenNavigator& navigator) = 0;
    };
} // namespace cpp_warships::game_tui
