#include <application/head/common/PresentationContext.h>
#include <application/head/tui/FtxuiNotices.h>
#include <application/head/tui/FtxuiPalette.h>
#include <application/head/tui/KeyHint.h>
#include <application/head/tui/SaveBrowserView.h>
#include <application/persistence/SaveArchive.h>

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head::tui {
    namespace {
        constexpr int MINIMUM_PANEL_WIDTH = 46;
    }  // namespace

    SaveBrowserView::SaveBrowserView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    ftxui::Element SaveBrowserView::renderElement() {
        const common::Theme& theme = context_.theme();
        const std::vector<persistence::SaveSummary> saves = context_.game().saves().savedMatches();
        const int chosen = context_.state().saves.selectedIndex;

        std::vector<ftxui::Element> rows{
            ftxui::text("SAVED GAMES") | ftxui::bold | color(theme.accent) | ftxui::hcenter,
            ftxui::separator() | color(theme.border)
        };

        if (saves.empty()) {
            rows.push_back(ftxui::text("nothing has been saved yet") | color(theme.textMuted));
        }

        for (std::size_t index = 0; index < saves.size(); ++index) {
            const bool isChosen = static_cast<int>(index) == chosen;
            ftxui::Element line = ftxui::text(
                " " + persistence::SaveArchive::momentOf(saves[index].id) + "   " +
                saves[index].name + " "
            );

            rows.push_back(
                isChosen ? std::move(line) | bgcolor(theme.accent) | color(theme.background)
                         : std::move(line) | color(theme.text)
            );
        }

        rows.push_back(ftxui::separator() | color(theme.border));

        std::vector<ftxui::Element> hints{keyHint(theme, "arrows", "choose a save")};
        if (!saves.empty()) {
            hints.push_back(keyHint(theme, "enter", "load it"));
            hints.push_back(keyHint(theme, "d", "delete it"));
        }
        hints.push_back(keyHint(theme, "esc", "back to the menu"));

        rows.push_back(keyLegend(std::move(hints)));
        rows.push_back(noticeBlock(theme, context_.application()));

        return ftxui::vbox(std::move(rows)) |
               ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, MINIMUM_PANEL_WIDTH) | ftxui::center |
               ftxui::border | color(theme.border) | bgcolor(theme.background) | ftxui::flex;
    }
}  // namespace cpp_warships::head::tui
