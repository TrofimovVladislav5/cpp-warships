#include <application/head/intents/UseSkillIntent.h>

#include <optional>

#include <application/head/intents/IntentContext.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    UseSkillIntent::UseSkillIntent(std::optional<core::Coordinate> target)
        : target_(target) {}

    void UseSkillIntent::applyTo(const IntentContext& context) const {
        context.application.game().play().useSkill(target_);
    }
} // namespace cpp_warships::head
