#include <application/head/input/EventBus.h>

#include <utility>

namespace cpp_warships::head {
    void EventBus::readScreenWith(const ScreenKind screen, std::unique_ptr<ScreenInput> input) {
        inputs_[screen] = std::move(input);
    }

    std::optional<model::GameEvent> EventBus::interpret(
            const ScreenKind screen,
            const Keystroke& stroke
    ) {
        const auto reader = inputs_.find(screen);
        if (reader == inputs_.end()) {
            return std::nullopt;
        }

        return reader->second->interpret(stroke);
    }

    model::EventScope scopeOf(const ScreenKind screen) noexcept {
        switch (screen) {
            case ScreenKind::Menu:
                return model::EventScope::Menu;
            case ScreenKind::Placement:
                return model::EventScope::Placement;
            case ScreenKind::Battle:
                return model::EventScope::Battle;
            case ScreenKind::GameOver:
                break;
        }

        return model::EventScope::Always;
    }
} // namespace cpp_warships::head
