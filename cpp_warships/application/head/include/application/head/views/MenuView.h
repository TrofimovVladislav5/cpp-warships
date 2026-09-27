#pragma once

#include <ftxui/dom/elements.hpp>

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/screens/MenuState.h>
#include <application/head/views/ftxui_bridge/FtxuiView.h>

namespace cpp_warships::head {
    /** @brief Draws the menu: the title, the board sizes on offer and the themes.
     *  Reads what it was given and returns elements; it changes nothing. */
    class MenuView final : public FtxuiView {
    public:
        MenuView(
                const Theme& theme,
                const MenuState& state,
                MatchInProgressQuery hasMatch,
                SavedMatchQuery hasSavedMatch
        );

        [[nodiscard]] InputEvent interpret(const Keystroke& stroke) const override;

    protected:
        [[nodiscard]] ftxui::Element renderElement() override;

    private:
        const Theme& theme_;
        const MenuState& state_;
        MatchInProgressQuery hasMatch_;
        SavedMatchQuery hasSavedMatch_;
    };
} // namespace cpp_warships::head
