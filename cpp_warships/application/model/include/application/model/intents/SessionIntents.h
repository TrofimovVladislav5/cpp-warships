#pragma once

#include <application/model/intents/GameIntent.h>

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::model::behaviors {
    class SaveBehavior;
}

namespace cpp_warships::model::intents {
    /** @brief Puts the match in play away. */
    class SaveMatchIntent final : public GameIntent {
    public:
        explicit SaveMatchIntent(behaviors::SaveBehavior& saves) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::SaveBehavior& saves_;
    };

    /** @brief Picks the saved match back up, in place of whatever was in play. */
    class LoadMatchIntent final : public GameIntent {
    public:
        explicit LoadMatchIntent(behaviors::SaveBehavior& saves) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::SaveBehavior& saves_;
    };

    /** @brief Marks the session as over. */
    class FinishSessionIntent final : public GameIntent {
    public:
        explicit FinishSessionIntent(ApplicationContext& application) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        ApplicationContext& application_;
    };
}  // namespace cpp_warships::model::intents
