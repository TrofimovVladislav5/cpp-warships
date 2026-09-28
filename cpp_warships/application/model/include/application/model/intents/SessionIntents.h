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
        SaveMatchIntent(behaviors::SaveBehavior& saves, std::string name) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::SaveBehavior& saves_;
        std::string name_;
    };

    /** @brief Picks a named save back up, in place of whatever was in play. */
    class LoadMatchIntent final : public GameIntent {
    public:
        LoadMatchIntent(behaviors::SaveBehavior& saves, std::string slot) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::SaveBehavior& saves_;
        std::string slot_;
    };

    /** @brief Throws a named save away. */
    class DeleteSaveIntent final : public GameIntent {
    public:
        DeleteSaveIntent(behaviors::SaveBehavior& saves, std::string slot) noexcept;

        [[nodiscard]] std::string name() const override;
        [[nodiscard]] IntentResult apply() const override;

    private:
        behaviors::SaveBehavior& saves_;
        std::string slot_;
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
