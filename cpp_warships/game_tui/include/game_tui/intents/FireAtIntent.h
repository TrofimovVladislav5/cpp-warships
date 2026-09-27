#pragma once

#include <game_core/Coordinate.h>
#include <game_tui/intents/Intent.h>

namespace cpp_warships::game_tui {
    /** @brief Asks the application to fire at a cell of the enemy's waters. */
    class FireAtIntent final : public Intent {
    public:
        explicit FireAtIntent(game_core::Coordinate coordinate);

        void applyTo(const IntentContext& context) const override;

    private:
        game_core::Coordinate coordinate_;
    };
} // namespace cpp_warships::game_tui
