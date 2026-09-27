#pragma once

#include <application/head/intents/Intent.h>

namespace cpp_warships::head {
    /** @brief Asks the application to begin a new match on a board of the given size. */
    class StartMatchIntent final : public Intent {
    public:
        explicit StartMatchIntent(int boardSize);

        void applyTo(const IntentContext& context) const override;

    private:
        int boardSize_;
    };
} // namespace cpp_warships::head
