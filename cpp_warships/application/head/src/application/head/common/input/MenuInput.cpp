#include <application/head/common/input/MenuInput.h>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace cpp_warships::head {
    namespace {
        constexpr int SMALLEST_BOARD_SIZE = 8;
        constexpr int LARGEST_BOARD_SIZE = 20;
        constexpr int BOARD_SIZE_STEP = 2;
    }  // namespace

    MenuInput::MenuInput(PresentationContext& context) noexcept : context_(context) {
    }

    std::optional<model::GameEvent> MenuInput::interpret(const Keystroke& stroke) {
        if (stroke.key == Key::ArrowLeft) {
            resizeBoard(-BOARD_SIZE_STEP);
            return std::nullopt;
        }
        if (stroke.key == Key::ArrowRight) {
            resizeBoard(BOARD_SIZE_STEP);
            return std::nullopt;
        }
        if (isCharacter(stroke, "t")) {
            cycleTheme();
            return std::nullopt;
        }
        if (stroke.key == Key::Enter) {
            return model::MatchStartRequested{.boardSize = context_.state().menu.selectedBoardSize};
        }
        if (isCharacter(stroke, "r")) {
            return model::MatchResumeRequested{};
        }
        if (isCharacter(stroke, "l")) {
            return model::MatchLoadRequested{};
        }
        if (isCharacter(stroke, "s")) {
            return model::MatchSaveAndQuitRequested{};
        }
        if (isCharacter(stroke, "q") || stroke.key == Key::Escape) {
            return model::SessionQuitRequested{};
        }

        return std::nullopt;
    }

    void MenuInput::resizeBoard(const int step) {
        context_.state().menu.selectedBoardSize = std::clamp(
            context_.state().menu.selectedBoardSize + step,
            SMALLEST_BOARD_SIZE,
            LARGEST_BOARD_SIZE
        );
    }

    void MenuInput::cycleTheme() {
        const std::vector<Theme>& themes = availableThemes();
        context_.state().menu.selectedThemeIndex =
            (context_.state().menu.selectedThemeIndex + 1) % static_cast<int>(themes.size());

        context_.themeSelection().change(
            themes[static_cast<std::size_t>(context_.state().menu.selectedThemeIndex)].name
        );
    }
}  // namespace cpp_warships::head
