#include <application/head/common/input/keys/MenuKeys.h>
#include <application/head/common/input/Keystroke.h>
#include <application/head/common/PresentationContext.h>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace cpp_warships::head::common::input::keys {
    namespace {
        constexpr int SMALLEST_BOARD_SIZE = 8;
        constexpr int LARGEST_BOARD_SIZE = 20;
        constexpr int BOARD_SIZE_STEP = 2;
    }  // namespace

    MenuKey::MenuKey(PresentationContext& context) noexcept
        : context_(context) {}

    bool ResizeBoardKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::ArrowLeft || stroke.key == Key::ArrowRight;
    }

    std::optional<model::events::GameEvent> ResizeBoardKey::interpret(const Keystroke& stroke) {
        const int step = stroke.key == Key::ArrowLeft ? -BOARD_SIZE_STEP : BOARD_SIZE_STEP;
        int& chosen = context_.state().menu.selectedBoardSize;

        chosen = std::clamp(chosen + step, SMALLEST_BOARD_SIZE, LARGEST_BOARD_SIZE);
        return std::nullopt;
    }

    bool CycleThemeKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "t");
    }

    std::optional<model::events::GameEvent> CycleThemeKey::interpret(const Keystroke&) {
        const std::vector<Theme>& themes = availableThemes();
        int& chosen = context_.state().menu.selectedThemeIndex;

        chosen = (chosen + 1) % static_cast<int>(themes.size());
        context_.themeSelection().change(themes[static_cast<std::size_t>(chosen)].name);

        return std::nullopt;
    }

    bool StartMatchKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::Enter;
    }

    std::optional<model::events::GameEvent> StartMatchKey::interpret(const Keystroke&) {
        return model::events::MatchStartRequested{
            .boardSize = context_.state().menu.selectedBoardSize
        };
    }

    bool ResumeMatchKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "r");
    }

    std::optional<model::events::GameEvent> ResumeMatchKey::interpret(const Keystroke&) {
        return model::events::MatchResumeRequested{};
    }

    bool LoadMatchKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "l");
    }

    std::optional<model::events::GameEvent> LoadMatchKey::interpret(const Keystroke&) {
        return model::events::MatchLoadRequested{};
    }

    bool SaveAndQuitKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "s");
    }

    std::optional<model::events::GameEvent> SaveAndQuitKey::interpret(const Keystroke&) {
        return model::events::MatchSaveAndQuitRequested{};
    }

    bool QuitKey::matches(const Keystroke& stroke) const {
        return isCharacter(stroke, "q") || stroke.key == Key::Escape;
    }

    std::optional<model::events::GameEvent> QuitKey::interpret(const Keystroke&) {
        return model::events::SessionQuitRequested{};
    }
}  // namespace cpp_warships::head::common::input::keys
