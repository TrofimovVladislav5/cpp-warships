#include <application/head/common/input/EventBus.h>
#include <application/head/common/input/Keystroke.h>

#include <utility>

namespace cpp_warships::head::common::input {
    void EventBus::readScreenWith(const ScreenKind screen, std::unique_ptr<ScreenInput> input) {
        inputs_[screen] = std::move(input);
    }

    std::optional<model::events::GameEvent> EventBus::interpret(
        const ScreenKind screen,
        const Keystroke& stroke
    ) {
        const auto reader = inputs_.find(screen);
        if (reader == inputs_.end()) {
            return std::nullopt;
        }

        return reader->second->interpret(stroke);
    }

    model::events::EventScope scopeOf(const ScreenKind screen) noexcept {
        switch (screen) {
            case ScreenKind::Menu:
                return model::events::EventScope::Menu;
            case ScreenKind::Saves:
                return model::events::EventScope::Saves;
            case ScreenKind::SaveNaming:
                return model::events::EventScope::SaveNaming;
            case ScreenKind::Placement:
                return model::events::EventScope::Placement;
            case ScreenKind::Battle:
                return model::events::EventScope::Battle;
        }

        return model::events::EventScope::Always;
    }
}  // namespace cpp_warships::head::common::input
