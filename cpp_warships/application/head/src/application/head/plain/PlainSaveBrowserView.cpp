#include <application/head/common/PresentationContext.h>
#include <application/head/common/render/Notices.h>
#include <application/head/plain/PlainFrame.h>
#include <application/head/plain/PlainSaveBrowserView.h>
#include <application/persistence/SaveArchive.h>

#include <cstddef>
#include <string>
#include <vector>

namespace cpp_warships::head::plain {
    PlainSaveBrowserView::PlainSaveBrowserView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    common::render::Frame PlainSaveBrowserView::render(int, int) {
        const std::vector<persistence::SaveSummary> saves = context_.game().saves().savedMatches();
        const int chosen = context_.state().saves.selectedIndex;

        std::vector<std::string> lines{
            "==============================",
            "         SAVED GAMES          ",
            "==============================",
            ""
        };

        if (saves.empty()) {
            lines.emplace_back("  nothing has been saved yet");
        }

        for (std::size_t index = 0; index < saves.size(); ++index) {
            const std::string marker = static_cast<int>(index) == chosen ? "  > " : "    ";
            lines.push_back(
                marker + persistence::SaveArchive::momentOf(saves[index].id) + "   " +
                saves[index].name
            );
        }

        lines.emplace_back("");
        lines.push_back(plainKeyLine("arrows", "choose a save"));
        if (!saves.empty()) {
            lines.push_back(plainKeyLine("enter", "load it"));
            lines.push_back(plainKeyLine("d", "delete it"));
        }
        lines.push_back(plainKeyLine("esc", "back to the menu"));

        for (const std::string& notice : common::render::noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return common::render::frameOfLines(lines);
    }
}  // namespace cpp_warships::head::plain
