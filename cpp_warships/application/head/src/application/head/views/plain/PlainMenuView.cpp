#include <application/head/views/plain/PlainMenuView.h>

#include <application/head/views/Notices.h>

#include <string>
#include <utility>
#include <vector>

#include <application/head/views/plain/PlainFrame.h>

namespace cpp_warships::head {
    PlainMenuView::PlainMenuView(const PresentationContext& context) noexcept
        : context_(context) {}


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
        }

        if (context_.game().saves().hasSavedMatch()) {
            lines.push_back(plainKeyLine("f3", "load the saved match"));
        }

        lines.push_back(plainKeyLine("q", "quit"));
        lines.emplace_back("");

        for (const std::string& notice : noticesToShow(context_.application())) {
            lines.push_back("  ! " + notice);
        }

        return frameOfLines(lines);
    }
} // namespace cpp_warships::head
