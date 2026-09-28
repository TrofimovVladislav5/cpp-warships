#pragma once

#include <application/head/common/input/InputKey.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input::keys {
    /** @brief Every binding here works from the context and nothing else. */
    class SaveNamingKey : public InputKey {
    public:
        explicit SaveNamingKey(PresentationContext& context) noexcept;

    protected:
        PresentationContext& context_;
    };

    /** @brief Adds a typed character to the name. */
    class TypeNameKey final : public SaveNamingKey {
    public:
        using SaveNamingKey::SaveNamingKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Rubs the last character out. */
    class EraseNameKey final : public SaveNamingKey {
    public:
        using SaveNamingKey::SaveNamingKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };

    /** @brief Puts the match away under the typed name, which must not be empty. */
    class ConfirmNameKey final : public SaveNamingKey {
    public:
        using SaveNamingKey::SaveNamingKey;

        [[nodiscard]] bool matches(const Keystroke& stroke) const override;
        [[nodiscard]] std::optional<model::events::GameEvent> interpret(
            const Keystroke& stroke
        ) override;
    };
}  // namespace cpp_warships::head::common::input::keys
