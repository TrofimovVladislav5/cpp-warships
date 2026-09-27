#include <game_tui/input/SaveMatchEventHandler.h>

#include <memory>
#include <utility>

#include <game_tui/intents/SaveMatchIntent.h>

namespace cpp_warships::game_tui {
    SaveMatchEventHandler::SaveMatchEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool SaveMatchEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::SaveKey;
    }

    void SaveMatchEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<SaveMatchIntent>());
    }
} // namespace cpp_warships::game_tui
