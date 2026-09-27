#pragma once

#include <ftxui/dom/elements.hpp>

#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/screens/MenuState.h>
#include <game_tui/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::game_tui {
    /** @brief Draws the menu: the title, the board sizes on offer and the themes.
     *  Reads what it was given and returns elements; it changes nothing. */
    class MenuView final : public FtxuiView {
    public:
        MenuView(const Theme& theme, const MenuState& state, MatchInProgressQuery hasMatch);

        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const Theme& theme_;
        const MenuState& state_;
        MatchInProgressQuery hasMatch_;
    };
} // namespace cpp_warships::game_tui
