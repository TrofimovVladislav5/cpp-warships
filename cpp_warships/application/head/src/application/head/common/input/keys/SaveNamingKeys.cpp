#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/Keystroke.h>
#include <application/head/common/input/keys/SaveNamingKeys.h>

namespace cpp_warships::head::common::input::keys {
    namespace {
        constexpr std::size_t LONGEST_NAME = 40;

        [[nodiscard]] bool isTypable(const Keystroke& stroke) {
            return stroke.key == Key::Character && stroke.character.size() == 1 &&
                   stroke.character.front() >= ' ';
        }
    }  // namespace

    SaveNamingKey::SaveNamingKey(PresentationContext& context) noexcept
        : context_(context) {}

    bool TypeNameKey::matches(const Keystroke& stroke) const {
        return isTypable(stroke) && context_.state().naming.typedName.size() < LONGEST_NAME;
    }

    std::optional<model::events::GameEvent> TypeNameKey::interpret(const Keystroke& stroke) {
        context_.state().naming.typedName += stroke.character;
        return std::nullopt;
    }

    bool EraseNameKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::Backspace || stroke.key == Key::Delete;
    }

    std::optional<model::events::GameEvent> EraseNameKey::interpret(const Keystroke&) {
        std::string& typed = context_.state().naming.typedName;
        if (!typed.empty()) {
            typed.pop_back();
        }

        return std::nullopt;
    }

    bool ConfirmNameKey::matches(const Keystroke& stroke) const {
        return stroke.key == Key::Enter && !context_.state().naming.typedName.empty();
    }

    std::optional<model::events::GameEvent> ConfirmNameKey::interpret(const Keystroke&) {
        return model::events::MatchSaveAndQuitRequested{.name = context_.state().naming.typedName};
    }
}  // namespace cpp_warships::head::common::input::keys
