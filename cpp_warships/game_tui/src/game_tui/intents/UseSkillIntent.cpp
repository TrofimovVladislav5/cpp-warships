#include <game_tui/intents/UseSkillIntent.h>

#include <optional>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    UseSkillIntent::UseSkillIntent(std::optional<game_core::Coordinate> target)
        : target_(target) {}

    void UseSkillIntent::applyTo(const IntentContext& context) const {
        context.session.useSkill(target_);
    }
} // namespace cpp_warships::game_tui
