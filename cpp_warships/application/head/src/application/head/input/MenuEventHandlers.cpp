#include <application/head/input/MenuEventHandlers.h>

#include <algorithm>
#include <cstddef>
#include <memory>
#include <utility>

#include <application/head/Theme.h>
#include <application/head/intents/ChangeThemeIntent.h>
#include <application/head/intents/LoadMatchIntent.h>
#include <application/head/intents/QuitIntent.h>
#include <application/head/intents/ResumeMatchIntent.h>
#include <application/head/intents/ShowScreenIntent.h>
#include <application/head/intents/StartMatchIntent.h>
#include <application/head/screens/ScreenKind.h>

namespace cpp_warships::head {
    namespace {
        constexpr int SMALLEST_BOARD_SIZE = 8;
        constexpr int LARGEST_BOARD_SIZE = 20;
        constexpr int BOARD_SIZE_STEP = 2;
    } // namespace

    BoardSizeEventHandler::BoardSizeEventHandler(MenuState& state)
        : state_(state) {}

    bool BoardSizeEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::ArrowLeft || input.stroke.key == Key::ArrowRight;
    }

    void BoardSizeEventHandler::handleEvent(const InputEvent& input) {
        const int step = input.stroke.key == Key::ArrowLeft ? -BOARD_SIZE_STEP : BOARD_SIZE_STEP;
        state_.selectedBoardSize = std::clamp(
                state_.selectedBoardSize + step,
                SMALLEST_BOARD_SIZE,
                LARGEST_BOARD_SIZE
        );
    }

    StartMatchEventHandler::StartMatchEventHandler(IntentSink intentSink, const MenuState& state)
        : intentSink_(std::move(intentSink))
        , state_(state) {}

    bool StartMatchEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::Enter;
    }

    void StartMatchEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<StartMatchIntent>(state_.selectedBoardSize));
    }

    ResumeMatchEventHandler::ResumeMatchEventHandler(
            IntentSink intentSink,
            MatchInProgressQuery hasMatch
    )
        : intentSink_(std::move(intentSink))
        , hasMatch_(std::move(hasMatch)) {}

    bool ResumeMatchEventHandler::isHandled(const InputEvent& input) const {
        return isCharacter(input.stroke, "r") && hasMatch_();
    }

    void ResumeMatchEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<ResumeMatchIntent>());
    }

    LoadMatchEventHandler::LoadMatchEventHandler(
            IntentSink intentSink,
            SavedMatchQuery hasSavedMatch
    )
        : intentSink_(std::move(intentSink))
        , hasSavedMatch_(std::move(hasSavedMatch)) {}

    bool LoadMatchEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::LoadKey && hasSavedMatch_();
    }

    void LoadMatchEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<LoadMatchIntent>());
    }

    CycleThemeEventHandler::CycleThemeEventHandler(IntentSink intentSink, MenuState& state)
        : intentSink_(std::move(intentSink))
        , state_(state) {}

    bool CycleThemeEventHandler::isHandled(const InputEvent& input) const {
        return isCharacter(input.stroke, "t");
    }

    void CycleThemeEventHandler::handleEvent(const InputEvent&) {
        const auto& themes = availableThemes();
        state_.selectedThemeIndex =
                (state_.selectedThemeIndex + 1) % static_cast<int>(themes.size());

        const auto index = static_cast<std::size_t>(state_.selectedThemeIndex);
        intentSink_(std::make_shared<ChangeThemeIntent>(themes[index].name));
    }

    QuitEventHandler::QuitEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool QuitEventHandler::isHandled(const InputEvent& input) const {
        return isCharacter(input.stroke, "q") || input.stroke.key == Key::Escape;
    }

    void QuitEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<QuitIntent>());
    }
} // namespace cpp_warships::head
