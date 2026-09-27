#include <game_tui/views/plain/PlainMenuView.h>

#include <string>
#include <utility>
#include <vector>

#include <game_tui/views/plain/PlainFrame.h>

namespace cpp_warships::game_tui {
    PlainMenuView::PlainMenuView(
            const Theme& theme,
            const MenuState& state,
            MatchInProgressQuery hasMatch,
            SavedMatchQuery hasSavedMatch
    )
        : theme_(theme)
        , state_(state)
        , hasMatch_(std::move(hasMatch))
        , hasSavedMatch_(std::move(hasSavedMatch)) {}

    InputEvent PlainMenuView::interpret(const Keystroke& stroke) const {
        return {.stroke = stroke};
    }

    Frame PlainMenuView::render(int, int) {
        const std::string boardSize = std::to_string(state_.selectedBoardSize);

        std::vector<std::string> lines{
                "==============================",
                "         CPP WARSHIPS         ",
                "==============================",
                "",
                "  board size : " + boardSize + " x " + boardSize + "   (left / right)",
                "  theme      : " + theme_.name + "   (t)",
                "",
                plainKeyLine("enter", "start a new match"),
        };

        if (hasMatch_()) {
            lines.push_back(plainKeyLine("r", "resume the match in play"));
        }

        if (hasSavedMatch_()) {
            lines.push_back(plainKeyLine("f3", "load the saved match"));
        }

        lines.push_back(plainKeyLine("q", "quit"));
        lines.emplace_back("");

        return frameOfLines(lines);
    }
} // namespace cpp_warships::game_tui
