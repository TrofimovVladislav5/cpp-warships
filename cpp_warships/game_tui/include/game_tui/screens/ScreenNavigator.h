#pragma once

#include <map>
#include <memory>

#include <game_tui/screens/Screen.h>
#include <game_tui/screens/ScreenKind.h>

namespace cpp_warships::game_tui {
    /** @brief Which screen is showing, and which ones there are to show.
     *  Holds no game state: it only knows where the player currently is. */
    class ScreenNavigator {
    public:
        void add(std::unique_ptr<Screen> screen);

        /** @brief Shows @p screen, if there is one registered under that name. */
        void showScreen(ScreenKind screen);

        [[nodiscard]] ScreenKind currentScreen() const noexcept;
        [[nodiscard]] bool knows(ScreenKind screen) const;
        [[nodiscard]] Screen& activeScreen();

    private:
        std::map<ScreenKind, std::unique_ptr<Screen>> screens_;
        ScreenKind currentScreen_ = ScreenKind::Menu;
    };
} // namespace cpp_warships::game_tui
