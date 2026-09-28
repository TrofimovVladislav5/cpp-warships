#include <application/head/common/PresentationContext.h>
#include <application/head/tui/FtxuiNotices.h>
#include <application/head/tui/FtxuiPalette.h>
#include <application/head/tui/KeyHint.h>
#include <application/head/tui/SaveNamingView.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head::tui {
    namespace {
        constexpr int MINIMUM_PANEL_WIDTH = 46;
    }  // namespace

    SaveNamingView::SaveNamingView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    ftxui::Element SaveNamingView::renderElement() {
        const common::Theme& theme = context_.theme();
        const std::string typed = context_.state().naming.typedName;

        std::vector<ftxui::Element> rows{
            ftxui::text("NAME THIS MATCH") | ftxui::bold | color(theme.accent) | ftxui::hcenter,
            ftxui::separator() | color(theme.border),
            ftxui::hbox(
                {ftxui::text("name  ") | color(theme.textMuted),
                 ftxui::text(typed + "_") | ftxui::bold | color(theme.text)}
            )
        };

        if (typed.empty()) {
            rows.push_back(
                ftxui::text("a name is needed before it can be put away") | color(theme.textMuted)
            );
        }

        rows.push_back(ftxui::separator() | color(theme.border));

        std::vector<ftxui::Element> hints{
            keyHint(theme, "letters", "type a name"),
            keyHint(theme, "back", "rub one out")
        };
        if (!typed.empty()) {
            hints.push_back(keyHint(theme, "enter", "save and quit"));
        }
        hints.push_back(keyHint(theme, "esc", "back to the menu"));

        rows.push_back(keyLegend(std::move(hints)));
        rows.push_back(noticeBlock(theme, context_.application()));

        return ftxui::vbox(std::move(rows)) |
               ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, MINIMUM_PANEL_WIDTH) | ftxui::center |
               ftxui::border | color(theme.border) | bgcolor(theme.background) | ftxui::flex;
    }
}  // namespace cpp_warships::head::tui
