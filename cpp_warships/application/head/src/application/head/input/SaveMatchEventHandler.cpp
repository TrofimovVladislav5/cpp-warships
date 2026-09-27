#include <application/head/input/SaveMatchEventHandler.h>

#include <memory>
#include <utility>

#include <application/head/intents/SaveMatchIntent.h>

namespace cpp_warships::head {
    SaveMatchEventHandler::SaveMatchEventHandler(IntentSink intentSink)
        : intentSink_(std::move(intentSink)) {}

    bool SaveMatchEventHandler::isHandled(const InputEvent& input) const {
        return input.stroke.key == Key::SaveKey;
    }

    void SaveMatchEventHandler::handleEvent(const InputEvent&) {
        intentSink_(std::make_shared<SaveMatchIntent>());
    }
} // namespace cpp_warships::head
