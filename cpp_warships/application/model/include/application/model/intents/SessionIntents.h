#pragma once

#include <application/model/ApplicationContext.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/model/intents/GameIntent.h>

namespace cpp_warships::model {
    /** @brief Puts the match in play away. */
    class SaveMatchIntent final : public GameIntent {
    public:
        explicit SaveMatchIntent(SaveBehavior& saves) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        SaveBehavior& saves_;
    };

    /** @brief Picks the saved match back up, in place of whatever was in play. */
    class LoadMatchIntent final : public GameIntent {
    public:
        explicit LoadMatchIntent(SaveBehavior& saves) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        SaveBehavior& saves_;
    };

    /** @brief Marks the session as over. The host watches for this rather than being told,
     *  which is what keeps quitting behind whatever was queued before it. */
    class FinishSessionIntent final : public GameIntent {
    public:
        explicit FinishSessionIntent(ApplicationContext& application) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        ApplicationContext& application_;
    };
} // namespace cpp_warships::model
