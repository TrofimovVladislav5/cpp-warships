#include <application/head/common/render/Notices.h>
#include <application/head/plain/PlainFrame.h>
#include <application/head/plain/PlainMenuView.h>

#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::head {
    PlainMenuView::PlainMenuView(const PresentationContext& context) noexcept : context_(context) {
    }

    Frame PlainMenuView::render(int, int) {
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
            lines.push_back(plainKeyLine("s", "save and quit"));
        }

        if (context_.game().saves().hasSavedMatch()) {
            lines.push_back(plainKeyLine("l", "load the saved match"));
        }

        lines.push_back(plainKeyLine("q", "quit"));
        lines.emplace_back("");

        for (const std::string& notice : noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return frameOfLines(lines);
    }
}  // namespace cpp_warships::head
