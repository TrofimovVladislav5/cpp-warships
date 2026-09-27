#include <application/head/views/MenuView.h>

#include <application/head/views/ftxui_bridge/FtxuiPalette.h>

#include <string>
#include <utility>
#include <vector>

#include <ftxui/component/event.hpp>

#include <application/head/views/KeyHint.h>
#include <application/head/views/ftxui_bridge/FtxuiNotices.h>

namespace cpp_warships::head {
    namespace {
        constexpr int MINIMUM_PANEL_WIDTH = 46;

        ftxui::Element titleBlock(const Theme& theme) {
            return ftxui::vbox(
                    {ftxui::text("CPP WARSHIPS") | ftxui::bold | color(theme.accent) |
                             ftxui::hcenter,
                     ftxui::text("a terminal fleet engagement") | color(theme.textMuted) |
                             ftxui::hcenter}
            );
        }
    } // namespace

    MenuView::MenuView(const PresentationContext& context) noexcept
        : context_(context) {}


    ftxui::Element MenuView::renderElement() {
        const Theme& theme = context_.theme();
        const MenuState& state = context_.state().menu;
        const bool hasMatchInProgress = context_.game().hasMatch();

        std::vector<ftxui::Element> rows{
                titleBlock(theme),
                ftxui::separator() | color(theme.border),
                ftxui::hbox(
                        {ftxui::text("board size  ") | color(theme.textMuted),
                         ftxui::text(
                                 std::to_string(state.selectedBoardSize) + " x " +
                                 std::to_string(state.selectedBoardSize)
                         ) | ftxui::bold |
                                 color(theme.text),
                         ftxui::text("   left right") | color(theme.textMuted)}
                ),
                ftxui::hbox(
                        {ftxui::text("theme       ") | color(theme.textMuted),
                         ftxui::text(theme.name) | ftxui::bold | color(theme.accent),
                         ftxui::text("   t") | color(theme.textMuted)}
                ),
                ftxui::separator() | color(theme.border),
        };

        std::vector<ftxui::Element> hints{keyHint(theme, "enter", "start a new match")};

        if (hasMatchInProgress) {
            hints.push_back(keyHint(theme, "r", "resume the match in play"));
        }

        if (context_.game().saves().hasSavedMatch()) {
            hints.push_back(keyHint(theme, "f3", "load the saved match"));
        }

        hints.push_back(keyHint(theme, "q", "quit"));
        rows.push_back(keyLegend(std::move(hints)));
        rows.push_back(noticeBlock(theme, context_.application()));

        return ftxui::vbox(std::move(rows)) |
               ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, MINIMUM_PANEL_WIDTH) | ftxui::center |
               ftxui::border | color(theme.border) | bgcolor(theme.background) | ftxui::flex;
    }
} // namespace cpp_warships::head
