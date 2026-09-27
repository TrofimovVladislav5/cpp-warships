#pragma once

#include <optional>

#include <application/core/Coordinate.h>
#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to spend the next banked skill.
     *  The target is carried for the skills that need one and left empty for those that do not. */
    class UseSkillIntent final : public Intent {
    public:
        explicit UseSkillIntent(std::optional<core::Coordinate> target);

        void applyTo(const IntentContext& context) const override;

    private:
        std::optional<core::Coordinate> target_;
    };
} // namespace cpp_warships::head
