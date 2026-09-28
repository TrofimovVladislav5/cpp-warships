#include <application/head/common/PresentationContext.h>
#include <application/head/common/render/Notices.h>
#include <application/head/plain/PlainFrame.h>
#include <application/head/plain/PlainMenuView.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head::plain {
    PlainMenuView::PlainMenuView(const common::PresentationContext& context) noexcept
        : context_(context) {}

    common::render::Frame PlainMenuView::render(int, int) {
        const std::string boardSize = std::to_string(context_.state().menu.selectedBoardSize);

        std::vector<std::string> lines{
            "==============================",
            "         CPP WARSHIPS         ",
            "==============================",
            "",
            "  board size : " + boardSize + " x " + boardSize + "   (left / right)",
            "  theme      : " + context_.theme().name + "   (t)",
            "",
            plainKeyLine("enter", "start a new match"),
        };

        if (context_.game().hasMatch()) {
            lines.push_back(plainKeyLine("r", "resume the match in play"));
            lines.push_back(plainKeyLine("s", "name it and quit"));
        }

        if (!context_.game().hasMatch() && context_.game().saves().hasSavedMatch()) {
            lines.push_back(plainKeyLine("l", "load a saved match"));
        }

        lines.push_back(plainKeyLine("q", "quit"));
        lines.emplace_back("");

        for (const std::string& notice : common::render::noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return common::render::frameOfLines(lines);
    }
}  // namespace cpp_warships::head::plain
