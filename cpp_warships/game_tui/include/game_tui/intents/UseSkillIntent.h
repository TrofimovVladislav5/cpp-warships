#pragma once

#include <optional>

#include <game_core/Coordinate.h>
#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to spend the next banked skill.
     *  The target is carried for the skills that need one and left empty for those that do not. */
    class UseSkillIntent final : public Intent {
    public:
        explicit UseSkillIntent(std::optional<game_core::Coordinate> target);

        void applyTo(const IntentContext& context) const override;

    private:
        std::optional<game_core::Coordinate> target_;
    };
} // namespace cpp_warships::game_tui
