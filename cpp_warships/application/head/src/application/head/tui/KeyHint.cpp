#include <application/head/common/Theme.h>
#include <application/head/tui/FtxuiPalette.h>
#include <application/head/tui/KeyHint.h>

#include <cstddef>
#include <utility>

namespace cpp_warships::head::tui {
    namespace {
        /** @brief How far a legend is held off the edges of its container, in columns. */
        constexpr int LEGEND_PADDING = 2;

        ftxui::Element padding() {
            return ftxui::text(std::string(static_cast<std::size_t>(LEGEND_PADDING), ' '));
        }
    }  // namespace

    ftxui::Element keyHint(
        const common::Theme& theme,
        const std::string& key,
        const std::string& description
    ) {
        return ftxui::hbox(
            {ftxui::text(" " + key + " ") | color(theme.background) | bgcolor(theme.accent),
             ftxui::filler(),
             ftxui::text(description) | color(theme.text)}
        );
    }

    ftxui::Element keyLegend(std::vector<ftxui::Element> hints) {
        return ftxui::hbox({padding(), ftxui::vbox(std::move(hints)) | ftxui::flex, padding()});
    }
}  // namespace cpp_warships::head::tui
